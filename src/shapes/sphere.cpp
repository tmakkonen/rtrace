#include "sphere.h"

Sphere::Sphere(const Shape::Material &m, const Vector &v, const double rad)
  : Shape(m), m_center(v) , m_radius(rad), m_radius_squared(rad*rad) {

}

Sphere::~Sphere() {
}

bool Sphere::intersect(const Ray &ray, double &t)  {
  Vector dist = m_center - ray.origin;
  double B = ray.direction * dist;
  double D = B*B - dist*dist + m_radius_squared;

  if (D < 0.0)
    return false;

  double D_sqrt = sqrtf(D);
  double t0 = B - D_sqrt;
  double t1 = B + D_sqrt;

  bool r = false;
  if ((t0 > 0.1) && (t0 < 1)) {
    t = t0;
    r = true;
  }
  if ((t1 > 0.1) && (t1 < t)) {
    t = t1;
    r = true;
  }
  
  return r;
}

