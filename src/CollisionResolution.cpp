#include "CollisionResolution.h"
#include <cmath>
#include "Moon.h"
#include "PeriodicDomain.h"

void elasticCollision(const PeriodicDomain& domain, Moon& moonA, Moon& moonB)
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
      const double ma = M_PI * moonA.r * moonA.r;
      const double mb = M_PI * moonB.r * moonB.r;
      const double eff_mass = 2.0 * ma * mb / (ma + mb);
      const double ux = du * dx + dv * dy;
      const double xx = dx * dx + dy * dy;
      const double ix = eff_mass * (ux / xx) * dx;
      const double iy = eff_mass * (ux / xx) * dy;
      moonA.u += ix / ma;
      moonA.v += iy / ma;
      moonB.u -= ix / mb;
      moonB.v -= iy / mb;
    }
  }
}
