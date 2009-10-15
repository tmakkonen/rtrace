#ifndef RTRACE_SCENEPARSER_H
#define RTRACE_SCENEPARSER_H
#include "shapes.h"
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

class Scene {
public:

  //! Material
  struct Material {
    string name;
    float reflection;
    float diffuse[3];

    // todo remove
    string str() const {
      stringstream ss;
      ss << name << " R:" << reflection << " diffuse:";
      for (int i = 0; i < 3; i++) ss << diffuse[i] << " ";
      return ss.str();
    } 
  };

  //! Camera
  struct Camera {
  };

  //! Viewport
  struct Viewport {
    unsigned width;
    unsigned height;
  };

  struct Light {
    Vector position;
    float intensity[3];
  };
  
  // ctor
  Scene(const string &scenefile);
  
  // dtor
  virtual ~Scene();
  
  bool parse();

  Viewport getViewport() { return m_viewport; }
  Camera getCamera() { return m_cam; }
  vector<Light> getLights() { return m_lights; }
  
  
private:
  string m_scenefile;
  Viewport m_viewport;
  Camera m_cam;

  //! material list
  vector<Material> m_materials;

  //! shape list
  vector<Shape> m_shapes;

  //! light source list
  vector<Light> m_lights;

};

#endif
//**// END OF FILE //**//
// vim: tw=78 sw=2 sts=2
