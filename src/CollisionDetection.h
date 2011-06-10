#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Ray.h"
#include "Moon.h"

struct Intersection
{
  double x;
  double y;
  double t;
};

bool intersectionRayMoon(const Ray& ray, const Moon& moon, Intersection& result);

#endif // COLLISIONDETECTION_H

