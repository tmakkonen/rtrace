#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include "sceneparser.h"
#include <rgb.h>
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;


// json helper:
// compare names
static bool same_name(const json::Pair &p, const string &name) {
  return p.name_ == name;
}

static Vector get_vector(const json::Array &a) {
  assert(a.size() == 3);

  return Vector(a[0].get_real(), a[1].get_real(), a[2].get_real());
}


// json helper:
// find an Object from the json scene file corresponding to the name
static const json::Value& get_value(const json::Object &o, string name) {

  json::Object::const_iterator i = find_if(
    o.begin(), o.end(), boost::lambda::bind(same_name, boost::lambda::_1, name));

  if (i == o.end()) return json::Value::null;

  return i->value_;
}


// json helper
// get a string value corresponding the key from the object
static string get_str(const json::Object &o, const string &name) {
  json::Value v = get_value(o, name);
  if (v.type() != json::str_type) {
    // todo error
  }

  return v.get_str();
}


// json helper
// get a float value corresponding to the key from the object
static float get_float(const json::Object &o, const string &name) {
  json::Value v = get_value(o, name);
  if (v.type() != json::real_type) {
    // todo error
  }

  return static_cast<float>(v.get_real());
}


Scene::Scene(const string &file) : m_scenefile(file) {
}


Scene::~Scene() {
}


int Scene::getMaterialIdx(const std::string &material) {
  int idx = -1;
  for (int i = 0; i < m_materials.size(); i++) {
    if (m_materials[i].name == material) {
      idx = i;
      break;
    }
  }
  assert(idx != -1);
  return idx;
}


bool Scene::parse() {

  // open and read the scene file
  ifstream scenehandle(m_scenefile.c_str());
  if (!scenehandle) {
    cerr << "error opening scene file: " << m_scenefile << endl;
    return false;
  }

  // read the scene file to a string
  string buf = string(
    std::istreambuf_iterator<char>(scenehandle),
    std::istreambuf_iterator<char>());

  scenehandle.close();
  
  // check that the file is a valid json one
  json::Value jsonval;
  try {
    json::read_or_throw(buf, jsonval);
  } catch (const json::Error_position &e) {
    cerr << "syntax error:" << e.line_ << ":" << e.column_ << endl;
    return false;
  }

  // read the data from the file
  json::Object root (jsonval.get_obj());

  string version = get_str(root, "version");

  // read the material list
  json::Array materials = get_value(root, "materials").get_array();

  for (int i = 0; i < materials.size(); i++) {
    json::Object &m = materials[i].get_obj();

    Material mat;
    // get material name
    mat.name = get_str(m, "name");
    
    // get material reflection coeff
    mat.reflection = get_float(m, "reflection");
    mat.power = get_value(m, "power").get_int();
    mat.refraction = get_float(m, "refraction");
    
    // get diffuse values
    json::Array diffuse1 = get_value(m, "diffuse1").get_array();
    json::Array diffuse2 = get_value(m, "diffuse2").get_array();

    // specular values
    json::Array specular = get_value(m, "specular").get_array();

    // material type
    std::string type = get_str(m, "type");
    if (type == "turbulence") 
      mat.type = Type_Turbulence;
    else if (type == "marble")
      mat.type = Type_Marble;
    else if (type == "default")
      mat.type = Type_Default;
    else {
      std::cout << "invalid material type " << type << std::endl;
      return false;
    }
      
    // todo check error
    assert(diffuse1.size() == 3 && diffuse2.size() == 3 && specular.size() == 3);
    mat.diffuse1 =
      RGB(diffuse1[0].get_real(), diffuse1[1].get_real(), diffuse1[2].get_real());
        mat.diffuse2 =
      RGB(diffuse2[0].get_real(), diffuse2[1].get_real(), diffuse2[2].get_real());
    mat.specular =
      RGB(specular[0].get_real(), specular[1].get_real(), specular[2].get_real());
    
    //cout << mat.str() << endl;
    m_materials.push_back(mat);
  }

  // descend into scene
  json::Object scene = get_value(root, "scene").get_obj();

  // viewport
  json::Object port = get_value(scene, "viewport").get_obj();
  m_viewport.width  = get_value(port, "width").get_int();
  m_viewport.height = get_value(port, "height").get_int();
  
  // camera

  // objects
  parseObjects(get_value(scene, "objects").get_array());

  // lights
  json::Array lights = get_value(scene, "lights").get_array();
  for (int i = 0; i < lights.size(); i++) {
    json::Object o = lights[i].get_obj();

    Light l;
    l.position = get_vector(get_value(o, "position").get_array());    
    json::Array intensity = get_value(o, "intensity").get_array();
    
    assert(intensity.size() == 3);
    l.intensity = RGB(intensity[0].get_real(), intensity[1].get_real(), intensity[2].get_real());

    m_lights.push_back(l);
  }
  
  return true;
}


void Scene::parseObjects(const json::Array &arr) {
  for (int i = 0; i < arr.size(); i++) {
    json::Object o = arr[i].get_obj();
    string type = get_str(o, "type");

    // create an object based on the object string
    if (type == "plane") {
      int idx = getMaterialIdx(get_str(o, "material"));
      Vector n = get_vector(get_value(o, "normal").get_array());
      float dist = get_float(o, "distance");
      m_shapes.push_back(new Plane(idx, n, dist));      
    }
    if (type == "sphere") {
      Vector v = get_vector(get_value(o, "center").get_array());
      float rad = get_float(o, "radius");
      int idx = getMaterialIdx(get_str(o, "material"));

      m_shapes.push_back(new Sphere(idx, v, rad));
    }
  }
}
