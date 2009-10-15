#ifndef RTRACE_RAY_H
#define RTRACE_RAY_H
#include "Vector.h"

class Ray {
  
public:
  //! direct access to vector data
  Vector origin;
  Vector direction;
  
  Ray(const Vector &orig, const Vector &dir) :
    origin(orig), direction(dir) {}
   
private:

};
#endif
