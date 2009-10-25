#include <plane.h>

Plane::Plane(const int material, const Vector &normal, const double dist)
  : Shape(material, Shape::Plane), m_N(normal), m_dist(dist) {
  //m_N.normalize();
}


bool Plane::intersect(const Ray &ray, double &distance) {
  double d = m_N * ray.direction;

  if (d > -0.000001 && d < 0.000001) return false;
    
  if (d != 0.0) {
    double dist = -((m_N * ray.origin) + m_dist) / d;
    if (dist > 0.0) {
      if (dist < distance) {
        distance = dist;
        return true;
      }
    }
  }

  return false;
}
