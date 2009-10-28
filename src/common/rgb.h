#ifndef RTRACE_RGB_H
#define RTRACE_RGB_H
#include <sstream>
class RGB {
public:
  float Red, Green, Blue;

  RGB(const float r, const float g, const float b) :
    Red(r), Green(g), Blue(b) {}
    
  RGB() : Red(0), Green(0), Blue(0) {}


  RGB& operator =(const RGB &rhs) {
    if (this == &rhs) return *this;
    Red = rhs.Red; Green = rhs.Green; Blue = rhs.Blue;
    return *this;
  }
    
  RGB(const std::vector<float> &v) {
    assert(v.size() == 3);      
    Red = v[0]; Green = v[1]; Blue = v[2];
  }

  RGB operator *(const RGB &rhs) {
    return RGB(Red * rhs.Red, Green*rhs.Green, Blue*rhs.Blue);
  }

  friend RGB operator *(const RGB &c, const float d) {
    return RGB(c.Red*d, c.Green*d, c.Blue*d);
  }

  friend RGB operator *(const float d, const RGB &c) {
    return c * d;
  }

  RGB& operator +=(const RGB &rhs) {
    Red += rhs.Red; Green += rhs.Green; Blue += rhs.Blue;
    return *this;
  }
  
  RGB operator +(const RGB &rhs) const {
    return RGB(*this) += rhs;
  }

  std::string str() const {
    std::stringstream ss;
    ss << "[" << Red << ", " << Green << ", " << Blue << "]";
    return ss.str();
  }
  
};

#endif
