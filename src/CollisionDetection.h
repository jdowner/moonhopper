#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Ray.h"
#include "Moon.h"

struct Intersection
{
  double x; // x co-ordinate of the interaction
  double y; // y co-ordinate of the interaction
  double t; // distance to the intersection point
};

bool intersectionRayMoon(const Ray& ray, const Moon& moon, Intersection& result);

#endif // COLLISIONDETECTION_H

