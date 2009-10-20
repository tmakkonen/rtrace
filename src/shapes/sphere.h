#ifndef RTRACE_SPHERE_H
#define RTRACE_SPHERE_H

#include "shape.h"

class Sphere : public Shape {
public:
  Sphere (const Material &m, const Vector &v, const double rad);
  virtual ~Sphere();

  // ray-sphere intersection impl.
  virtual double getIntersection(const Ray &r);

private:
  Vector m_center;
  double m_radius;
};

#endif
