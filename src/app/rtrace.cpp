#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <rtrace.h>
#include <rgb.h>
#include <boost/shared_ptr.hpp>
#include <perlin.h>

using std::cerr;
using std::cout;
using std::endl;

#define TRACEDEPTH 6
#define EPSILON 0.001f

//typedef boost::thread thread;
typedef boost::shared_ptr<boost::thread> thread_ptr;

template <typename T>
static void swrite(FILE *f, T value, size_t n = 1) {
  for (int i = 0; i < n; i++) 
    fwrite(&value, sizeof(T), 1, f);
}

rtrace::tgafile::tgafile(const string &fname, const unsigned x, const unsigned y,
                         const int chunks) {
  m_file = fopen(fname.c_str(), "wb");
  if (!m_file) throw std::runtime_error("could not open output file");
  m_x = x; m_y = y;
  // append TGA header
  // id + color map type
  swrite<char>(m_file, 0, 2);

  // rgb image type
  swrite<char>(m_file, 2);

  // dummy color map;
  swrite<char>(m_file, 0, 5);

  // image specs
  swrite<short>(m_file, 0, 2);
  swrite<short>(m_file, m_x);
  swrite<short>(m_file, m_y);
  swrite<char>(m_file, 24);
  swrite<char>(m_file, 0);

  m_buf.resize(chunks);
}


void rtrace::tgafile::add_chunk(int index, std::vector<RGB> &b) {
  boost::mutex::scoped_lock l(m_mutex);
  for (int i = 0; i < b.size(); i++) {
    m_buf[index].push_back(b[i]);
  }
}


rtrace::tgafile::~tgafile() {
  fclose(m_file);
}


void rtrace::tgafile::write() const {
  // dump the actual image data buffer to file
  for (int x = 0; x < m_buf.size(); x++)
    for (int i = 0; i < m_buf[x].size(); i++) {    
    swrite<unsigned char>(m_file, (unsigned char)std::min(m_buf[x][i].Blue*255.0, 255.0));
    swrite<unsigned char>(m_file, (unsigned char)std::min(m_buf[x][i].Green*255.0, 255.0));
    swrite<unsigned char>(m_file, (unsigned char)std::min(m_buf[x][i].Red*255.0, 255.0));
  }
}


bool rtrace::traceparams::validate() {
  if (scenefile.empty()) {
    cerr << "please define a file name" << endl;
    return false;
  }
  
  return true;
}


rtrace::rtrace(traceparams *params) : m_params(params) {

}

void shade(Ray &ray, Shape *prim, Vector &pi, Vector &ldir, Scene::Light &light,
           Scene::Material &m, float coeff, RGB &acc) {

  Vector N = prim->getNormal(pi);
  float dot = ldir * N;
  
  // add diffuse component based on material type.
  switch (m.type) {
  case Scene::Type_Default:
  {
    // diffuse component
    if (dot > 0) {
      acc += coeff * dot * m.diffuse1 * light.intensity;
    }    
  }    
  break;
  case Scene::Type_Turbulence:
  {
    float noise = 0.0;
    for (int level = 1; level < 10; level ++) {
      float foo[3] = {pi.x * level*5 , pi.y * level*5, pi.z * level*5};
      noise += (1.0f / level ) 
        * fabsf(float(textures::perlin::instance().noise3(foo)));
    }
    
    float lambert = dot * coeff;
    acc+= coeff * (lambert * light.intensity) *
      (noise * m.diffuse1 + (1.0 - noise) * m.diffuse2);
  }
  break;
  case Scene::Type_Marble:
  {
    float noise = 0.0;
    for (int level = 1; level < 10; level ++) {
      float foo[3] = {pi.x * level*7 , pi.y * level*7, pi.z * level*7};
      noise += (1.0f / level ) 
        * fabsf(float(textures::perlin::instance().noise3(foo)));
    }
    
    float lambert = dot * coeff;
    noise = 0.5 * sinf((pi.x + pi.y) * 10.0f + noise) + 0.5;

    acc += coeff * (lambert * light.intensity) *
      (noise * m.diffuse1 + (1.0 - noise) * m.diffuse2);

  }
  break;
  } // end switchcase

  // specular component
  Vector R = ldir - 2.0f * dot * N;
  dot = ray.direction * R;
  if (dot > 0.0f) {
    float spec = coeff * powf(dot, m.power);
    acc += m.specular * light.intensity * spec;
  }
}


void trace(Ray &ray, Scene *scene, RGB &color_acc, float refl_coeff, int trace_depth,
           float &distance) {

  // recursion end condition
  if (trace_depth > TRACEDEPTH)
    return;

  distance = 1000000.0;
  Scene::ShapeList shapes = scene->getObjects();
  
  // find the closest object intersecting with the ray
  Shape::Intersect result;
  Scene::ShapeList::iterator closest = shapes.end();
  for (Scene::ShapeList::iterator it = shapes.begin(); it < shapes.end(); it++) {
    Shape::Intersect temp = (*it)->intersect(ray, distance);
    if (temp != Shape::Miss) {
      result = temp;
      closest = it;
    }
  }

  // no hit found, we can terminate the ray 
  if (closest == shapes.end())
    return;

  if ((*closest)->hasNormal()) {
    
    // get the ray-primitive intersection point
    Vector p = ray.origin + ray.direction * distance;

    // get the primitive normal vector on the intersection point
    Vector N = (*closest)->getNormal(p);
    
    Scene::Material mat = scene->getMaterial((*closest)->getMaterialIndex());

    // iterate the light sources
    for (Scene::LightList::iterator it = scene->getLights().begin();
         it < scene->getLights().end(); it++) {
      
      Scene::Light l = *it;
      Vector L = l.position - p;      
      L.normalize();

      Ray light;
      light.origin = p + L * EPSILON;
      light.direction = L;
      
      bool inshadow = false;
      
      // calculate color components for each light source
      for(Scene::ShapeList::iterator it = shapes.begin(); it < shapes.end(); it++) {
        float t = 10000.0;
        if ((*it)->intersect(light, t) != Shape::Miss) {
          inshadow = true;
          break;
        }                  
      }

      // not in shadow, calculate lighting
      if (!inshadow) {              
        shade(ray, *closest, p, L, l, mat, refl_coeff, color_acc);
      }
    } 
    
    // calculate reflection
    float refl = mat.reflection;
    if (refl > 0.0 && trace_depth < TRACEDEPTH) {

      // reflection vector
      Vector R = ray.direction - 2.0f * (ray.direction * N) * N;

      // reflection ray, starting form the point of intersection
      Ray r(p + R * EPSILON, R);
      
      RGB reflection_color;

      // trace the reflection recursively
      trace(r, scene, reflection_color, refl_coeff*mat.reflection, trace_depth + 1, distance);
      color_acc += refl * reflection_color * mat.specular;
    }
    
    // calculate refraction
    // TODO not working
    float refr = mat.refraction;
    if (refr > 0.0  && trace_depth < TRACEDEPTH) {

      float rindex = 1.5; // TODO
      float n = refl_coeff / rindex;
      N = N * (float)result;
      float cosI = -(N * ray.direction);
      float cosT2 = 1.0f - N * N * (1.0f - cosI * cosI);
      
      if (cosT2 > 0.0) {
        
        Vector T = (n * ray.direction) + (n * cosI - sqrtf(cosT2)) * N;
        RGB color(0, 0, 0);
        float dist;
        Ray r(p + T * EPSILON, T);
        
        trace(r, scene, color, rindex, trace_depth + 1, dist);

        // beer's law
        RGB absorb = color * 0.015 * -dist;
        RGB transp(expf(absorb.Red), expf(absorb.Green), expf(absorb.Blue));

        color_acc += color * transp;
      }
    }
    //*/
  }
}
         

void do_job(int id, rtrace::tgafile *f, Scene *s, std::vector<RGB> &buf, int y1, int y2) {

  int scene_x = s->getViewport().width;
  int scene_y = s->getViewport().height;

  // TODO viewport in world coordinates
  float X1 = -4, X2 = 4, Y1 = -3, Y2 = 3;
  float dX = (X2 - X1) / scene_x;
  float dY = (Y2 - Y1) / scene_y;
  
  Vector origin(0, 0, -5);

  int idx = 0;
  
  float Y = Y1 + y1*dY;

  for (int y = y1; y < y2; y++) {
    float X = X1;
    
    for (int x = 0; x < scene_x; x++) {

      // supersampling for each pixel
      RGB color;
      for (int dx = -1; dx < 2; dx++)
        for (int dy = -1; dy < 2; dy++) {
          
          Vector dir = Vector(X + dX * dx / 2.0f, Y + dY * dy / 2.0f, 0) - origin;
          dir.normalize();

          Ray ray(origin, dir);
          float dist;
  
          trace(ray, s, color, 1.0, 0, dist);

          /*
          float ratio = 0.25;
          float exposure = -1.0;      
          t.Red = (1.0 - expf(t.Red * exposure)) * ratio;
          t.Green = (1.0 - expf(t.Green * exposure)) * ratio;
          t.Blue = (1.0 - expf(t.Blue * exposure)) * ratio;
          */
          
        }
      buf[idx++] = color*0.11;
      X += dX;
    }
    
    Y += dY;
  }

  // write our chunk to file.
  f->add_chunk(id, buf);
}


// job structure for a worker thread
struct trace_job {
  int id; // thread id.
  int y1; // y start
  int y2; // y end 
  std::vector<RGB> buf;  // result buffer.
  Scene *s;
  rtrace::tgafile *f;
  
  trace_job(int id, rtrace::tgafile *f, Scene *s, const std::vector<RGB> &b, int y1, int y2) :
    id(id), f(f), s(s), buf(b), y1(y1), y2(y2) {}

  void operator()() {
    do_job(id, f, s, buf, y1, y2);
  }
};


int rtrace::run() {

  // load the scene file
  Scene *scene = new Scene(m_params->scenefile);
  if (!scene->parse()) {
    cout << "scene file parsing failed" << endl;
    return EXIT_FAILURE;
  } 

  int nth = m_params->threads;
 
  std::vector<thread_ptr> threads;

  
  int width = scene->getViewport().width;
  int height = scene->getViewport().height;
  std::vector<std::vector<RGB> > bufs(nth);
  for (int i = 0; i < nth; i++)
    bufs[i].resize(width *height / nth);
  

  tgafile *f = new tgafile(m_params->outfile, width, height, nth);

  for (int i = 0; i < nth; i++) {
    int start = i*height/nth;
    int end = start + height/nth;
    threads.push_back(thread_ptr(new boost::thread(trace_job(i, f, scene, bufs[i], start, end))));
    if (m_params->verbose)
      std::cout << "spawned thread #" << i << std::endl;
  }

  for (int i = 0; i < threads.size(); i++) {
    threads[i]->join();
    if (m_params->verbose) 
      std::cout << "joined thread #" << i << std::endl;
  }

  if (m_params->verbose)
    std::cout << "trace done, write to file..." << std::endl;

  
  f->write();

  delete(scene);
  
  return EXIT_SUCCESS;
}
