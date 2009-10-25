#ifndef RTRACE_CYLINDER_H
#define RTRACE_CYLINDER_H

#include <shape.h>

class Cylinder : public Shape {
public:
  /*! ctor:
    \param i material index
    \param cap cap point
    \param base base point
    \param rad cylinder radius
  */
  Cylinder (const int i, const Vector &cap, Vector &base,  const double rad);
  virtual ~Sphere();

  // ray-cylinder intersection impl.
  virtual bool intersect(const Ray &r, double &t);

  virtual bool hasNormal() const { return true; }

  virtual Vector getNormal(const Vector &v) const;
  
private:
  Vector m_cap;
  Vector m_base;
  double m_radius;
  double m_radius_squared;

  Cylinder() {}
};

#endif
