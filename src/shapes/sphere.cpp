#include "sphere.h"

Sphere::Sphere(const int i, const Vector &v, const float rad)
  : Shape(i, Shape::Sphere), m_center(v) , m_radius(rad), m_radius_squared(rad*rad) {

}

Sphere::~Sphere() {
}

Shape::Intersect Sphere::intersect(const Ray &ray, float &t)  {
  Vector dist = m_center - ray.origin;
  float B = ray.direction * dist;
  float D = B*B - dist*dist + m_radius_squared;

  Shape::Intersect r = Shape::Miss;
  if (D < 0.0)
    return r;

  float D_sqrt = sqrtf(D);
  float t0 = B - D_sqrt;
  float t1 = B + D_sqrt;

  if ((t0 > 0.1) && (t0 < t)) {
    t = t0;
    r = r = Shape::InPrimitive;
  }
  if ((t1 > 0.1) && (t1 < t)) {
    t = t1;
    r = Shape::Hit;;
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

