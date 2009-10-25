#include "sphere.h"

Sphere::Sphere(const int i, const Vector &v, const double rad)
  : Shape(i, Shape::Sphere), m_center(v) , m_radius(rad), m_radius_squared(rad*rad) {

}

Sphere::~Sphere() {
}

bool Sphere::intersect(const Ray &ray, double &t)  {
  Vector dist = m_center - ray.origin;
  double B = ray.direction * dist;
  double D = B*B - dist*dist + m_radius_squared;

  if (D < 0.0)
    return false;

  if (D > -0.000001 && D < 0.000001) return false;

  double D_sqrt = sqrtf(D);
  double t0 = B - D_sqrt;
  double t1 = B + D_sqrt;

  bool r = false;
  if ((t0 > 0.1) && (t0 < t)) {
    t = t0;
    r = true;
  }
  if ((t1 > 0.1) && (t1 < t)) {
    t = t1;
    r = true;
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

