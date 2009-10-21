#include <memory>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "rtrace.h"

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


void RTrace::OutFile::setPixel(const float Rcoeff, const float Gcoeff, const float Bcoeff ) {
  unsigned char R = (unsigned char)std::min(Rcoeff*255.0, 255.0);
  unsigned char G = (unsigned char)std::min(Gcoeff*255.0, 255.0);
  unsigned char B = (unsigned char)std::min(Bcoeff*255.0, 255.0);
  m_buf.push_back(B);
  m_buf.push_back(G);
  m_buf.push_back(R);
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
  if (file.empty()) {
    cerr << "please define a file name" << endl;
    return false;
  }
  
  return true;
}


RTrace::RTrace(Params *params) : m_params(params) {
}


int RTrace::run() {

  // load the scene file
  Scene *scene = new Scene(m_params->file);
  if (!scene->parse()) {
    cout << "scene file parsing failed" << endl;
    return EXIT_FAILURE;
  } 

  int scene_x = scene->getViewport().width;
  int scene_y = scene->getViewport().height;
  
  OutFile f("test.tga", scene_x, scene_y);

  Scene::ShapeList shapes = scene->getObjects();

  for (int y = 0; y < scene_y; y++) {
    for (int x = 0; x < scene_x; x++) {
      float red = 0, green = 0, blue = 0;

      // view ray
      Ray r (Vector(double(x), double(y), -500.0), Vector(0.0, 0.0, 1.0));

      double t = 5000.0;
      // find the closest object intersecting with the ray
      Scene::ShapeList::iterator closest = shapes.end();
      for (Scene::ShapeList::iterator it = shapes.begin(); it < shapes.end(); it++) {
        if ((*it)->intersect(r, t)) {
          red = 0.5; green = 0.1; blue = 0.4;
          closest = it;
        }
      }
      
      if (closest != shapes.end()) {
        Shape::Material *mat = (*closest)->getMaterial();
      }

      f.setPixel(red, green, blue);
    }
  }
  
  f.Write();
  return EXIT_SUCCESS;
}
