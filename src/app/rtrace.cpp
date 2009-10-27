#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <rtrace.h>
#include <rgb.h>

using std::cerr;
using std::cout;
using std::endl;

#define TRACEDEPTH 6

template <typename T>
static void swrite(FILE *f, T value, size_t n = 1) {
  for (int i = 0; i < n; i++) 
    fwrite(&value, sizeof(T), 1, f);
}


RTrace::OutFile::OutFile(const string &fname, const unsigned x, const unsigned y) {
  m_file = fopen(fname.c_str(), "wb");
  if (!m_file) throw std::runtime_error("could not open output file");
  m_x = x; m_y = y;
}


RTrace::OutFile::~OutFile() {
  fclose(m_file);
}


void RTrace::OutFile::setPixel(const RGB &c) {
  m_buf.push_back((unsigned char)std::min(c.Blue*255.0, 255.0));
  m_buf.push_back((unsigned char)std::min(c.Green*255.0, 255.0));
  m_buf.push_back((unsigned char)std::min(c.Red*255.0, 255.0));

}


void RTrace::OutFile::Write() const {
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

  // dump the actual image data buffer to file
  for (int i = 0; i < m_buf.size(); i++)
    swrite<unsigned char>(m_file, m_buf[i]);
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


void RTrace::traceRay(Ray &ray, Scene *scene, RGB &color_acc, float refl_index, int trace_depth,
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
    for (Scene::LightList::iterator l = scene->getLights().begin();
         l < scene->getLights().end(); l++) {
      
      Vector L = l->position - p;      
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
          color_acc += dot * mat.diffuse * l->intensity;          
        }

        // specular component
        Vector R = L - 2.0f * dot * N;
        dot = ray.direction * R;
        if (dot > 0.0f) {
          float spec = powf(dot, mat.power);

          // add the specular color component to color accumulator
          color_acc += mat.specular * l->intensity * spec;
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
      traceRay(r, scene, reflection_color, refl_index, trace_depth + 1, distance);
      color_acc += refl * reflection_color * mat.specular;
    }

    ///*
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
        
        traceRay(r, scene, color, rindex, trace_depth + 1, dist);

        // beer's law
        RGB absorb = mat.specular * 0.15 * -dist;
        RGB transp(expf(absorb.Red), expf(absorb.Green), expf(absorb.Blue));

        color_acc += color * transp;
      }
    }
    //*/
  }
}

void RTrace::init(Scene *scene) {

  // TODO:
  // sceen plane in world coordinates
  m_X1 = -4, m_X2 = 4, m_Y1 = -3, m_Y2 = 3;

  // interpolation deltas
  m_dX = (m_X2 - m_X1) / scene->getViewport().width;
  m_dY = (m_Y2 - m_Y1) / scene->getViewport().height; 
  
}

int RTrace::run() {

  // load the scene file
  Scene *scene = new Scene(m_params->scenefile);
  if (!scene->parse()) {
    cout << "scene file parsing failed" << endl;
    return EXIT_FAILURE;
  } 

  init(scene);

  int width = scene->getViewport().width;
  int height = scene->getViewport().height;
  OutFile f(m_params->outfile, width, height);

  Vector origin(0, 0, -5);
  float Y = m_Y1;
  
  for (int y = 0; y < height; y++) {
    float X = m_X1;

    for (int x = 0; x < width; x++) {
      RGB acc;
      
      // supersampling for each pixel
      for (int dx = -1; dx < 2; dx++)
        for (int dy = -1; dy < 2; dy++) {
          RGB color;
          Vector dir = Vector(X + m_dX * dx / 2.0f, Y + m_dY * dy / 2.0f, 0) - origin;
          dir.normalize();
          Ray ray(origin, dir);
          float dist;
          traceRay(ray, scene, color, 1.0, 0, dist);

          // exposure function
          float ratio = 0.25;
          float exposure = -0.3;      
          color.Red = (1.0 - expf(color.Red * exposure)) * ratio;
          color.Green = (1.0 - expf(color.Green * exposure)) * ratio;
          color.Blue = (1.0 - expf(color.Blue * exposure)) * ratio;

          acc += color;            
        }
     
      f.setPixel(acc);
      X += m_dX;
    }
    
    Y += m_dY;
  } 

  // trace done, write to file
  f.Write();
  
  delete(scene);
  return EXIT_SUCCESS;
}
