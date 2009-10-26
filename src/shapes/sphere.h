#ifndef RTRACE_SPHERE_H
#define RTRACE_SPHERE_H

#include <shape.h>

class Sphere : public Shape {
public:
  Sphere (const int i, const Vector &v, const float rad);
  virtual ~Sphere();

  // ray-sphere intersection impl.
  virtual Intersect intersect(const Ray &r, float &t);

  virtual bool hasNormal() const { return true; }

  virtual Vector getNormal(const Vector &v) const;
  
private:
  Vector m_center;
  float m_radius;
  float m_radius_squared; 
};

#endif
