#include <plane.h>

Plane::Plane(const int material, const Vector &normal, const float dist)
  : Shape(material, Shape::Plane), m_N(normal), m_dist(dist) {
  m_N.normalize();
}


Shape::Intersect Plane::intersect(const Ray &ray, float  &distance) {
  float d = m_N * ray.direction;
    
  if (d != 0.0) {
    float dist = -((m_N * ray.origin) + m_dist) / d;
    if (dist > 0.0) {
      if (dist < distance) {
        distance = dist;
        return Shape::Hit;
      }
    }
  }

  return Shape::Miss;
}
