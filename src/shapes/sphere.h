#ifndef RTRACE_SPHERE_H
#define RTRACE_SPHERE_H

#include <shape.h>

class Sphere : public Shape {
public:
  Sphere (const int i, const Vector &v, const double rad);
  virtual ~Sphere();

  // ray-sphere intersection impl.
  virtual bool intersect(const Ray &r, double &t);

  virtual bool hasNormal() const { return true; }

  virtual Vector getNormal(const Vector &v) const;
  
private:
  Vector m_center;
  double m_radius;
  double m_radius_squared; 
};

#endif
