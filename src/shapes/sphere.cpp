#include "sphere.h"

Sphere::Sphere(const Shape::Material &m, const Vector &v, const double rad)
  : Shape(m), m_center(v) , m_radius(rad)
{
}

Sphere::~Sphere() 
{
}

double getIntersection(const Ray &ray) 
{
}

