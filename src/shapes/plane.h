#ifndef RTRACE_PLANE_H
#define RTRACE_PLANE_H

#include <shape.h>

class Plane : public Shape {
public:
  /*! ctor:
    \param i material index
    \param normal plane normal vector
    \param dist plane distance
  */
  Plane (const int i, const Vector &normal,  const float dist);
  virtual ~Plane() {}

  // ray-cylinder intersection impl.
  virtual Intersect intersect(const Ray &r, float &t);

  virtual bool hasNormal() const {
    return true;
  }

  virtual Vector getNormal(const Vector &v) const { 
    return m_N;
  };
  
private:
  Vector m_N;
  float m_dist;

};

#endif
