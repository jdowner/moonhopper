#include "CollisionResolution.h"
#include <cmath>
#include "Moon.h"
#include "PeriodicDomain.h"
#include "DataStore.h"

namespace
{
  double sign(double value)
  {
    if (value == 0.0) return 0.0;
    return (value < 0.0) ? -1.0 : 1.0;
  }
}

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

      // currently assuming that collisions always happen at the very edge of
      // the moons. This is wrong but it might be good enough.
      const double radialVelocityA = moonA.r * moonA.dtheta;
      const double radialVelocityB = moonB.r * moonB.dtheta;
      const double tangentialVelocity = (du * dy - dv * dx) / sqrt(xx);
      const double angularImpulse =
        radialVelocityA + radialVelocityB + tangentialVelocity;
      const double friction = DataStore::get<double>("Friction", 1.0);
      const double gamma = sign(angularImpulse);

      resolution.angularImpulseA = friction * gamma * sqrt(ix * ix + iy * iy);
      resolution.angularImpulseB = friction * gamma * sqrt(ix * ix + iy * iy);

      // Determine the location
      // TODO the location of the collision is approximate. In the future, it
      // may be worth considering whether a more accurate approach is
      // worthwhile.
      resolution.location.x = moonA.x + dx * moonA.r / sqrt(xx);
      resolution.location.y = moonA.y + dy * moonA.r / sqrt(xx);
    }
  }
}
