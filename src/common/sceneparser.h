#ifndef RTRACE_SCENEPARSER_H
#define RTRACE_SCENEPARSER_H

#include <string>
#include <vector>
#include <sstream>

#include "shapes.h"
#include <json_spirit.h>

namespace json = json_spirit;

using std::string;
using std::stringstream;

class Scene {
public:

  //! Material
  struct Material {
    string name;
    float reflection;
    std::vector<double> diffuse;
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


  typedef std::vector<Shape*> ShapeList;
  typedef std::vector<Material> MaterialList;
  typedef std::vector<Light> LightList;
  
  // ctor
  Scene(const string &scenefile);
  
  // dtor
  virtual ~Scene();
  
  bool parse();

  Viewport getViewport() { return m_viewport; }
  Camera getCamera() { return m_cam; }
  LightList getLights() { return m_lights; }
    
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

};

#endif
//**// END OF FILE //**//
// vim: tw=78 sw=2 sts=2
