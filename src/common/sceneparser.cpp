#include "sceneparser.h"
#include "json_spirit.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>

namespace json = json_spirit;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;


// json helper:
// compare names
static bool same_name(const json::Pair &p, const string &name) {
  return p.name_ == name;
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
// get a double value corresponding to the key from the object
static double get_double(const json::Object &o, const string &name) {
  json::Value v = get_value(o, name);
  if (v.type() != json::real_type) {
    // todo error
  }

  return v.get_real();
}


Scene::Scene(const string &file) : m_scenefile(file) {
}


Scene::~Scene() {
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
    mat.reflection = get_double(m, "reflection");
    
    // get diffuse values
    json::Array diffuse = get_value(m, "diffuse").get_array();
    
    // todo check error
    for(int j = 0; j < 3; j++)
      mat.diffuse[j] = diffuse[j].get_real();

    //cout << mat.str() << endl;
    m_materials.push_back(mat);
  }

  return true;
}

