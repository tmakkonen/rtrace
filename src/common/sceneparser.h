#ifndef RTRACE_SCENEPARSER_H
#define RTRACE_SCENEPARSER_H

#include <string>
#include <vector>
#include <sstream>

#include "shapes.h"
#include <json_spirit.h>
#include <rgb.h>
namespace json = json_spirit;

using std::string;
using std::stringstream;

class Scene {
public:

  //! Material
  struct Material {
    string name;
    float reflection;
    float refraction;
    int power;
    RGB diffuse;
    RGB specular;

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
    RGB intensity;
    Light()
    : position(0,0,0), intensity(0,0,0) {}
    Light(Vector v, RGB r)
    : position(v), intensity(r) {}
  };

  typedef std::vector<Shape*> ShapeList;
  typedef std::vector<Material> MaterialList;
  typedef std::vector<Light> LightList;
  
  // ctor
  Scene(const string &scenefile);
  
  // dtor
  virtual ~Scene();
  
  bool parse();

  Material getMaterial(const int i) {
    return m_materials[i];
  }
  
  Viewport getViewport() {
      return m_viewport;
  }
    
  Camera getCamera() {
    return m_cam;
  }
  
  LightList &getLights() {
    return m_lights;
  }
  
  ShapeList getObjects() {
    return m_shapes;
  }
  
private:
  string m_scenefile;
  Viewport m_viewport;
  Camera m_cam;

  //! material list
  MaterialList m_materials;

  //! shape list
  ShapeList m_shapes;

  //! light source list
  LightList m_lights;

  void parseObjects(const json::Array &arr);

  int getMaterialIdx(const std::string &name);
};

#endif
//**// END OF FILE //**//
// vim: tw=78 sw=2 sts=2
