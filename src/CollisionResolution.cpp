#include "CollisionResolution.h"
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
      const double alpha = (du*dx+dv*dy) / (dx*dx+dy*dy);
      const double nu = alpha * dx;
      const double nv = alpha * dy;
      moonA.u += nu;
      moonA.v += nv;
      moonB.u -= nu;
      moonB.v -= nv;
    }
  }
}
