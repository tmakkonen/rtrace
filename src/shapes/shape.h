#ifndef RTRACE_SHAPE_H
#define RTRACE_SHAPE_H
#include <vector>
#include <cassert>
#include <ray.h>
#include <rgb.h>

//! base class for all shapes
class Shape {
public:
  //! shape type
  typedef enum {
    Sphere,
    Plane
  } Type;
  

  typedef enum {
    InPrimitive = -1, 
    Miss = 0,
    Hit = 1
  } Intersect;
    
  Shape(const int i, Type t) : m_material_index(i), m_type(t) {}

  virtual ~Shape() {}

  Type type() { return m_type; }
  
  int getMaterialIndex() const {
    return m_material_index;
  }
    
  //! each child must provide own intersection impl.
  virtual Intersect intersect(const Ray &r, float &t) = 0;

  // does this shape support the geometric notion
  // of a normal. default impl returns true.
  virtual bool hasNormal() const { return true; }

  virtual Vector getNormal(const Vector &v) const = 0;
  
protected:
  int m_material_index;
  Type m_type;
};

#endif
