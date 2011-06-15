#include "Vector2d.h"
#include <cmath>

double deg2rad(double degrees)
{
  return degrees * 0.01745329251994329576;
}

Vector2d rotate(double degrees, const Vector2d& pivot, const Vector2d& vector)
{
  Vector2d result;

  const double x = vector.x - pivot.x;
  const double y = vector.y - pivot.y;
  const double c = cos(deg2rad(degrees));
  const double s = sin(deg2rad(degrees));

  result.x = pivot.x + c * x + s * y;
  result.y = pivot.y - s * x + c * y;

  return result;
}
