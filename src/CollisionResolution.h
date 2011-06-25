#ifndef COLLISIONRESOLUTION_H
#define COLLISIONRESOLUTION_H

#include "Vector2d.h"

class Moon;
class PeriodicDomain;

struct CollisionResolution
{
  enum Type
  {
    NONE,
    COLLISION
  };

  CollisionResolution()
  {
    type = NONE;
  }

  Vector2d impulseA;
  Vector2d impulseB;
  double angularImpulseA;
  double angularImpulseB;
  Type type;
};

void elasticCollision(const PeriodicDomain& domain, Moon& moonA, Moon& moonB, CollisionResolution& resolution);

#endif // COLLISIONRESOLUTION_H

