#ifndef RTRACE_SPHERE_H
#define RTRACE_SPHERE_H

#include "shape.h"

class Sphere : public Shape {
  public:
    Sphere (const Material &m);
    virtual ~Sphere();

    // ray-sphere intersection impl.
    virtual double getIntersection(const Ray &r);

  private:
};

#endif
