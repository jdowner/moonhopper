#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "Vector2d.h"

template <typename T, typename S>
double dot(const T& vectorA, const S& vectorB)
{
  return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

Vector2d rotate(double radians, const Vector2d& pivot, const Vector2d& vector);

#endif // MATHUTILS_H

