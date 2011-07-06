#ifndef AVATAR_H
#define AVATAR_H

#include "Moon.h"
#include "Ray.h"

struct Avatar
{
  double theta;
  double height;
  Moon* moon;
  bool isJumping;
  bool isDead;
  Ray up;
};

#endif // AVATAR_H

