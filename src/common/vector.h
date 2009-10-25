//! \file vector.h
//! \brief a simple 3D vector implementation
#ifndef RTRACE_VECTOR_H
#define RTRACE_VECTOR_H 

#include <math.h>
#include <iostream>
#include <sstream>

class Vector {
public:
  Vector(const double X, const double Y, const double Z) :
  x(X), y(Y), z(Z) {}
  
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
  
  const Vector operator -(const Vector &rhs) const {
    return Vector(*this) -= rhs;
  }
  
  Vector& operator +=(const Vector &rhs) {
    x+=rhs.x; y+=rhs.y; z+=rhs.z;
    return *this;
  }
  
  const Vector operator +(const Vector& rhs) const {
    return Vector(*this) += rhs;
  }
  
  double operator *(const Vector &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }
  
  friend Vector operator *(const Vector&v, const double d) {
    return Vector(v.x*d, v.y*d, v.z*d);
  }
  
  double length() const {
    return sqrt(x*x + y*y + z*z);
  }
  
  void normalize() {
    double l = x*x + y*y + z*z;
    if (l == 0) return;
    double scale = 1 / sqrtf(l);
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

  std::string str() const {
    std::stringstream ss;
    ss << "[" << x << ", " << y << ", " << x <<"]";
    return ss.str();
  }
  
  double x, y, z;
};

#endif
