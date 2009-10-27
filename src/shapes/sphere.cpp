#include "sphere.h"

Sphere::Sphere(const int i, const Vector &v, const float rad)
  : Shape(i, Shape::Sphere), m_center(v) , m_radius(rad), m_radius_squared(rad*rad) {

}

Sphere::~Sphere() {
}

Shape::Intersect Sphere::intersect(const Ray &ray, float &dist)  {

  Vector V = ray.origin - m_center;
  float  B = -(ray.direction * V);
  float  D = B*B - V*V + m_radius_squared;
 
  Shape::Intersect r = Shape::Miss;
  if (D < 0.0)
    return r;
 
  D = sqrtf(D);
  float t0 = B - D;
  float t1 = B + D;

  /*
  if ((t0 > 0.1) && (t0 < dist)) {
    dist = t0;
    r = Shape::Hit;
  }
  if ((t1 > 0.1) && (t1 < dist)) {
    dist = t1;  
    r = Shape::InPrimitive;    
  }
          
  return r;
  */


  /*
  Vector V = ray.origin - m_center;
  float B = -(V * ray.direction);
  float D = B*B - V*V + m_radius_squared;
  
  Shape::Intersect r = Shape::Miss;
  if (D < 0.0)
    return r;
 
  float D_sqrt = sqrtf(D);
  float t0 = B - D_sqrt;
  float t1 = B + D_sqrt;
  */
  if (t1 > 0.01f) {

    if (t0 < 0.0f) {
      if (t1 < dist) {
        dist = t1;
        r = Shape::InPrimitive;
      }
    }
    else {
      if (t0 < dist) {
        dist = t0;
        r = Shape::Hit;
      }
    }
  }
    
  return r;

}


// the normal vector of a point on a sphere is the connecting vector
// from the centre to the point normalized to unity.
Vector Sphere::getNormal(const Vector &v) const {
  
  Vector n = v - m_center;
  n.normalize();
  return n;  
}

