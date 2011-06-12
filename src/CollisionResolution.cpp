#include "CollisionResolution.h"
#include <cmath>
#include "Moon.h"
#include "PeriodicDomain.h"

void elasticCollision(const PeriodicDomain& domain, Moon& moonA, Moon& moonB, CollisionResolution& resolution)
{
  const double r = moonA.r + moonB.r;

  double dx = moonB.x - moonA.x;
  double dy = moonB.y - moonA.y;

  if (dx < -r)
  {
    dx += domain.getWidth();
  }

  if (dx > r)
  {
    dx -= domain.getWidth();
  }

  if (dy < -r)
  {
    dy += domain.getHeight();
  }

  if (dy > r)
  {
    dy -= domain.getHeight();
  }

  if (dx * dx + dy * dy < r * r)
  {
    const double du = moonB.u - moonA.u;
    const double dv = moonB.v - moonA.v;
    if (du * dx + dv * dy <= 0.0)
    {
      const double eff_mass = 2.0 * moonA.m * moonB.m / (moonA.m + moonB.m);
      const double ux = du * dx + dv * dy;
      const double xx = dx * dx + dy * dy;
      const double ix = eff_mass * (ux / xx) * dx;
      const double iy = eff_mass * (ux / xx) * dy;
      resolution.impulseA.x = ix;
      resolution.impulseA.y = iy;
      resolution.impulseB.x = ix;
      resolution.impulseB.y = iy;
      resolution.type = CollisionResolution::COLLISION;
    }
  }
}
