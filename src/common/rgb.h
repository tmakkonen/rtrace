#ifndef RTRACE_RGB_H
#define RTRACE_RGB_H

class RGB {
public:
  double Red;
  double Green;
  double Blue;

  RGB(const double r, const double g, const double b) :
    Red(r), Green(g), Blue(b) {}
    
  RGB() : Red(0), Green(0), Blue(0) {}


  RGB& operator =(const RGB &rhs) {
    if (this == &rhs) return *this;
    Red = rhs.Red; Green = rhs.Green; Blue = rhs.Blue;
    return *this;
  }
    
  RGB(const std::vector<double> &v) {
    assert(v.size() == 3);      
    Red = v[0]; Green = v[1]; Blue = v[2];
  }

  RGB operator *(const RGB &rhs) {
    return RGB(Red * rhs.Red, Green*rhs.Green, Blue*rhs.Blue);
  }

  friend RGB operator *(const RGB &c, const double d) {
    return RGB(c.Red*d, c.Green*d, c.Blue*d);
  }

  friend RGB operator *(const double d, const RGB &c) {
    return c * d;
  }

  RGB& operator +=(const RGB &rhs) {
    Red += rhs.Red; Green += rhs.Green; Blue += rhs.Blue;
    return *this;
  }
  
  RGB operator +(const RGB &rhs) const {
    return RGB(*this) += rhs;
  }

};

#endif
