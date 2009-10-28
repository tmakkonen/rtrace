#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <rtrace.h>
#include <rgb.h>
#include <boost/shared_ptr.hpp>

using std::cerr;
using std::cout;
using std::endl;

#define TRACEDEPTH 6

//typedef boost::thread thread;
typedef boost::shared_ptr<boost::thread> thread_ptr;

template <typename T>
static void swrite(FILE *f, T value, size_t n = 1) {
  for (int i = 0; i < n; i++) 
    fwrite(&value, sizeof(T), 1, f);
}

RTrace::OutFile::OutFile(const string &fname, const unsigned x, const unsigned y,
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



void RTrace::OutFile::add_chunk(int index, std::vector<RGB> &b) {
  boost::mutex::scoped_lock l(m_mutex);
  for (int i = 0; i < b.size(); i++) {
    m_buf[index].push_back(b[i]);
  }
}


RTrace::OutFile::~OutFile() {
  fclose(m_file);
}


void RTrace::OutFile::write() const {
  // dump the actual image data buffer to file
  for (int x = 0; x < m_buf.size(); x++)
    for (int i = 0; i < m_buf[x].size(); i++) {    
    swrite<unsigned char>(m_file, (unsigned char)std::min(m_buf[x][i].Blue*255.0, 255.0));
    swrite<unsigned char>(m_file, (unsigned char)std::min(m_buf[x][i].Green*255.0, 255.0));
    swrite<unsigned char>(m_file, (unsigned char)std::min(m_buf[x][i].Red*255.0, 255.0));
  }
}


bool RTrace::Params::validate() {
  if (scenefile.empty()) {
    cerr << "please define a file name" << endl;
    return false;
  }

  if (outfile.empty()) {
    outfile = "out.tga";
  }
  
  return true;
}


RTrace::RTrace(Params *params) : m_params(params) {

}

  
void trace(Ray &ray, Scene *scene, RGB &color_acc, float refl_index, int trace_depth,
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
  if (closest == shapes.end()) return;

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
      light.origin = p + L * 0.0001f;
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

      // not in shadow, calculate diffuse and specular color components
      if (!inshadow) {
        
        // diffuse component
        float dot = L * N;
        if (dot > 0.0f) {

          // add the diffuse component to color accumulator
          color_acc += dot * mat.diffuse * l.intensity;          
        }

        // specular component

        Vector R = L - 2.0f * dot * N;
        dot = ray.direction * R;
        if (dot > 0.0f) {
          float spec = powf(dot, mat.power);

          // add the specular color component to color accumulator
          color_acc += mat.specular * l.intensity * spec;
        }
        
      }
    } // light source loop end
    
    // calculate reflection
    float refl = mat.reflection;
    if (refl > 0.0 && trace_depth < TRACEDEPTH) {

      // reflection vector
      Vector R = ray.direction - 2.0f * (ray.direction * N) * N;

      // reflection ray, starting form the point of intersection
      Ray r(p + R*0.00001, R);
      
      RGB reflection_color(0, 0, 0);

      // trace the reflection recursively
      trace(r, scene, reflection_color, refl_index, trace_depth + 1, distance);
      color_acc += refl * reflection_color * mat.specular;
    }

    /*
    // calculate refraction
    float refr = mat.refraction;
    if (refr > 0.0  && trace_depth < TRACEDEPTH) {

      float rindex = 1.5; // TODO
      float n = refl_index / rindex;
      N = N * (float)result;
      float cosI = -(N * ray.direction);
      float cosT2 = 1.0f - ((N * N) * (1.0f - cosI * cosI));
      
      if (cosT2 > 0.0) {
        
        Vector T = (n * ray.direction) + (n * cosI - sqrtf(cosT2)) * N;
        RGB color(0, 0, 0);
        float dist;
        Ray r(p + T * 0.00001, T);
        
        trace(r, scene, color, rindex, trace_depth + 1, dist);

        // beer's law
        RGB absorb = mat.specular * 0.15 * -dist;
        RGB transp(expf(absorb.Red), expf(absorb.Green), expf(absorb.Blue));

        color_acc += color * transp;
      }
    }
    //*/
  }
}
         

void go(int id, RTrace::OutFile *f, Scene *s, std::vector<RGB> &buf, int y1, int y2) {

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

      //RGB acc;
      // supersampling for each pixel
      for (int dx = -1; dx < 2; dx++)
        for (int dy = -1; dy < 2; dy++) {
          RGB color;
          
          Vector dir = Vector(X + dX * dx / 2.0f, Y + dY * dy / 2.0f, 0) - origin;
          dir.normalize();

          Ray ray(origin, dir);
          float dist;
          trace(ray, s, color, 1.0, 0, dist);
          
          // exposure function
          float ratio = 0.25;
          float exposure = -0.3;      
          color.Red = (1.0 - expf(color.Red * exposure)) * ratio;
          color.Green = (1.0 - expf(color.Green * exposure)) * ratio;
          color.Blue = (1.0 - expf(color.Blue * exposure)) * ratio;

          buf[idx] += color;
        }

      idx++;
      X += dX;
    }
    
    Y += dY;
  }

  // write our chunk to file.
  f->add_chunk(id, buf);
}


struct trace_job {
  int id;
  int y1;
  int y2;
  std::vector<RGB> buf;
  Scene *s;
  RTrace::OutFile *f;
  
  trace_job(int id, RTrace::OutFile *f, Scene *s, const std::vector<RGB> &b, int y1, int y2) :
    id(id), f(f), s(s), buf(b), y1(y1), y2(y2) {}

  void operator()() {
    go(id, f, s, buf, y1, y2);
  }
};


int RTrace::run() {

  // load the scene file
  Scene *scene = new Scene(m_params->scenefile);
  if (!scene->parse()) {
    cout << "scene file parsing failed" << endl;
    return EXIT_FAILURE;
  } 

  std::vector<thread_ptr> threads;

  std::vector<RGB> buf1(640*480/2);
  std::vector<RGB> buf2(640*480/2);

  int width = scene->getViewport().width;
  int height = scene->getViewport().height;
  OutFile *f = new OutFile(m_params->outfile, width, height, 2);
  
  threads.push_back(thread_ptr(new boost::thread(trace_job(0, f, scene, buf1, 0, 480/2))));
  threads.push_back(thread_ptr(new boost::thread(trace_job(1, f, scene, buf2, 480/2+1, 480))));
                                 
  for (int i = 0; i < threads.size(); i++) 
    threads[i]->join();


  // trace done, write to file
  f->write();

  delete(scene);
  
  return EXIT_SUCCESS;
}
