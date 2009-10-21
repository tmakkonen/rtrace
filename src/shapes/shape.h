#ifndef RTRACE_SHAPE_H
#define RTRACE_SHAPE_H
#include <ray.h>
#include <vector>
#include <cassert>
//! base class for all shapes
class Shape {
public:
  
  struct RGB {
    double Red;
    double Green;
    double Blue;

    RGB(const std::vector<double> &v) {
      assert(v.size() == 3);      
      Red = v[0]; Green = v[1]; Blue = v[2];
    }
  };

  //! shape type
  typedef enum {
    Sphere
  } Type;
  
  //! material
  struct Material {
    float reflection;
    RGB diffuse;
    Material(const float r, RGB &d ) :reflection(r), diffuse(d)  {}
  };
  
  Shape(const Material &m) {}
  virtual ~Shape() {}

  Type type() { return m_type; }
  
  // material getter and setter
  Material* getMaterial() const {
    return m_mat;
  }
  
  void setMaterial(Material *m) {
    m_mat = m;
  }
  
  // each child must provide own intersection impl.
  virtual bool intersect(const Ray &r, double &t) = 0;
  
protected:
  Material *m_mat;
  Type m_type;
};

#endif
