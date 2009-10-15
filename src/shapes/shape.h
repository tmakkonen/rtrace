#ifndef RTRACE_SHAPE_H
#define RTRACE_SHAPE_H
#include <Ray.h>
//! base class for all shapes
class Shape {
  public:
    
    //! material
    struct Material {
    };
  
    Shape(const Material &m) {}
    virtual ~Shape() {}

    // material getter and setter
    Material* getMaterial() const {
      return m_mat;
    }

    void setMaterial(Material *m) {
      m_mat = m;
    }

    // each child must provide own intersection impl.
    virtual double getIntersection(const Ray &r) = 0;
    
  protected:
    Material *m_mat;

};

#endif
