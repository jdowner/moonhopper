#include "CollisionDetection.h"
#include <cmath>
#include <algorithm>

bool intersectionRayMoon(const Ray& ray, const Moon& moon, Intersection& result)
{
  const double rx = ray.ox - moon.x;
  const double ry = ray.oy - moon.y;

  const double b = rx * ray.nx + ry * ray.ny;
  const double c = rx * rx + ry * ry - moon.r * moon.r;

  if ((c > 0.0) && (b > 0.0))
  {
    return false;
  }

  const double desc = b * b - c;
  if (desc < 0.0)
  {
    return false;
  }

  result.t = std::max(0.0, -b - sqrt(desc));

  result.x = ray.ox + result.t * ray.nx;
  result.y = ray.oy + result.t * ray.ny;

  return true;
}

