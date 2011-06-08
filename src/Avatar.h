#ifndef AVATAR_H
#define AVATAR_H

#include "Moon.h"

struct Avatar
{
  double theta;
  double height;
  Moon* current;
};

#endif // AVATAR_H

