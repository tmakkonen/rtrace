#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <rtrace.h>
#include <rgb.h>

using std::cerr;
using std::cout;
using std::endl;

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


RGB RTrace::traceRay(Ray &ray, Scene *scene) {
  RGB color(0, 0, 0);
  double coef = 1.0;
  int level = 0;
  Scene::ShapeList shapes = scene->getObjects();
  
  do {
    // find the closest object intersecting with the ray
    double dist = 2000.0;    
    Scene::ShapeList::iterator closest = shapes.end();
    for (Scene::ShapeList::iterator it = shapes.begin(); it < shapes.end(); it++) {
      if ((*it)->intersect(ray, dist)) 
        closest = it;
    }

    if (closest == shapes.end())
      break;
        
    // we found an object intersecting this viewray
    
    // get the normal at that point, if supported
    if ((*closest)->hasNormal()) {
      
      // get the intersection point
      Vector p = ray.origin + ray.direction * dist;            
      Vector N = (*closest)->getNormal(p);
      
      Scene::Material mat = scene->getMaterial((*closest)->getMaterialIndex());
      
      // iterate the light sources
      for (Scene::LightList::iterator l = scene->getLights().begin();
           l < scene->getLights().end(); l++) {

        Vector dist = l->position - p;
        if (N * dist <= 0.0) {
          continue;
        }
        
        double t = sqrtf(dist * dist);
        if (t <= 0.0)
          continue;
        
        Ray light;
        light.origin = p;
        light.direction = dist * (1/t);
        
        bool inshadow = false;
        for(Scene::ShapeList::iterator it = shapes.begin(); it < shapes.end(); it++) {
          if ((*it)->intersect(light, t)) {          
            inshadow = true;
            break;
          }                  
        }
        if (inshadow)
          continue;
        
        // diffuse component
        double lambert = (light.direction * N) * coef;
        color += lambert * mat.diffuse * l->intensity;
        
        // blinn-phong
        Vector blinnDirection = light.direction - ray.direction;
        double temp = sqrtf(blinnDirection * blinnDirection);
        if (temp != 0.0) {
          blinnDirection = blinnDirection * (1/temp);
          double blinnTerm = coef * std::max(blinnDirection * N, 0.0);
          double power = powf(blinnTerm, mat.power);
          color += mat.specular * power * l->intensity;
        }        
      }

      coef *= mat.reflection;
      double reflet = 2.0 * (ray.direction * N);
      ray.origin = p;
      ray.direction = ray.direction - (N * reflet);
      level++;

    }    
  } while (coef > 0.0 && level < 10);
  
  return color;
}

int RTrace::run() {

  // load the scene file
  Scene *scene = new Scene(m_params->scenefile);
  if (!scene->parse()) {
    cout << "scene file parsing failed" << endl;
    return EXIT_FAILURE;
  } 

  int scene_x = scene->getViewport().width;
  int scene_y = scene->getViewport().height;
  
  OutFile f(m_params->outfile, scene_x, scene_y);

  for (int y = 0; y < scene_y; y++) {
    for (int x = 0; x < scene_x; x++) {

      RGB color;
      for (float xf = float(x); xf < x + 1.0; xf += 0.25) {
        for (float yf = float(y); yf < y + 1.0; yf += 0.25) {
          float ratio = 0.25;
          Ray r (Vector(xf, yf, -1000), Vector(0.0, 0.0, 1.0));
          RGB val = traceRay(r, scene);

          double exposure = -0.3;
          val.Red = (1.0 - expf(val.Red * exposure)) * ratio;
          val.Green = (1.0 - expf(val.Green * exposure)) * ratio;
          val.Blue = (1.0 - expf(val.Blue * exposure)) * ratio;

          color += val;
          // view ray
//          Ray r (Vector(double(x), double(y), -1000.0), Vector(0.0, 0.0, 1.0));
          
        }
      }
      f.setPixel(color);
    }
  }
 
  f.Write();
  
  delete(scene);
  return EXIT_SUCCESS;
}
