//! \file vector.h
//! \brief a simple 3D vector implementation
#ifndef RTRACE_VECTPR_H
#define RTRACE_VECTOR_H 

#include <math.h>
#include <iostream>
class Vector {
  public:
    Vector(const double x, const float y, const float z) :
      x(x), y(y), z(z) {}

    Vector() : x(0), y(0), z(0) {}

    Vector& operator =(const Vector &rhs) {
      if (this == &rhs) 
        return *this;
      x = rhs.x; y = rhs.y; z = rhs.z;
      return *this;
    }

    Vector& operator -=(const Vector &rhs) {
      x -= rhs.x; y -= rhs.y; z -= rhs.z;
      return *this;
    }

    const Vector operator -(const Vector &rhs) {
      return Vector(*this) -= rhs;
    }

    Vector& operator +=(const Vector &rhs) {
      x+=rhs.y; y+=rhs.y; z+=rhs.z;
      return *this;
    }

    const Vector operator +(const Vector& rhs) {
      return Vector(*this) += rhs;
    }

    double operator *(const Vector &rhs) {
      return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    friend Vector operator *(const Vector&v, double d) {
      return Vector(v.x*d, v.y*d, v.z*d);
    }

    double length() const {
      return sqrt(x*x + y*y + z*z);
    }

    void normalize() {
      double l = x*x+y*y+z*z;
      if (l == 0) return;
      double scale = 1 / sqrt(l);
      x *= scale;
      y *= scale;
      z *= scale;
    }
    
    Vector cross_prod(const Vector &v) const {
      return Vector(
          y * v.z - z * v.y,
          z * v.x - x * v.z,
          x * v.y - y * v.x);
    }

    double x, y, z;
};

std::ostream& operator <<(std::ostream &out, const Vector &v) {
  out << "x:" << v.x << " y:" << v.y << " z:" << v.z;
  return out;
}
#endif

// EOF
// vim: tw=78 sw=2 sts=2
