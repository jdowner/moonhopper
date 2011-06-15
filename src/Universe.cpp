#include "Universe.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>
#include <set>
#include <boost/foreach.hpp>
#include "DataStore.h"
#include "Ray.h"
#include "CollisionDetection.h"
#include "CollisionResolution.h"
#include "UpdateContext.h"
#include "MathUtils.h"

namespace
{
  double uniform()
  {
    return double(rand()) / RAND_MAX;
  }

  double wrap(double lo, double hi, double x)
  {
    assert(lo < hi);
    const double diff = hi - lo;
    while (x < lo) x += diff;
    while (x > hi) x -= diff;
    return x;
  }
}

Universe::Universe()
: m_avatarAngularSpeed(DataStore::get<double>("AvatarAngularSpeed", 0.05))
, m_domain(
    DataStore::get<double>("DomainMinX", -300.0),
    DataStore::get<double>("DomainMaxX", 300.0),
    DataStore::get<double>("DomainMinY", -300.0),
    DataStore::get<double>("DomainMaxY", 300.0))
{
  // For now we are just generating some random positions for the moons
  
  const unsigned int NUM_MOONS = DataStore::get<int>("NumberOfMoons", 32);
  const double MIN_X = DataStore::get<double>("DomainMinX", -300.0);
  const double MAX_X = DataStore::get<double>("DomainMaxX", 300.0);
  const double MIN_Y = DataStore::get<double>("DomainMinY", -300.0);
  const double MAX_Y = DataStore::get<double>("DomainMaxY", 300.0);
  const double MAX_VELOCITY = DataStore::get<double>("MaxVelocity", 30.0);
  const double MIN_ANGULAR_VELOCITY = 
    DataStore::get<double>("MinAngularVelocity", -M_PI);
  const double MAX_ANGULAR_VELOCITY = 
    DataStore::get<double>("MaxAngularVelocity", M_PI);
  const double MIN_RADIUS = DataStore::get<double>("MinMoonRadius", 5.0);
  const double MAX_RADIUS = DataStore::get<double>("MaxMoonRadius", 30.0);

  for (unsigned int i = 0; i < NUM_MOONS; ++i)
  {
    Moon* moon = new Moon;

    moon->r = MIN_RADIUS + (MAX_RADIUS - MIN_RADIUS) * uniform();
    moon->m = M_PI * moon->r * moon->r;
    moon->x = MIN_X + (MAX_X - MIN_X) * uniform();
    moon->y = MIN_Y + (MAX_Y - MIN_Y) * uniform();
    moon->theta = 2.0 * M_PI * uniform();
    moon->dtheta = MIN_ANGULAR_VELOCITY + 
      (MAX_ANGULAR_VELOCITY - MIN_ANGULAR_VELOCITY) * uniform();

    const double vAngle = 2.0 * M_PI * uniform();
    const double vMagnitude = MAX_VELOCITY * uniform();
    moon->u = vMagnitude * cos(vAngle);
    moon->v = vMagnitude * sin(vAngle);

    m_moons.push_back(moon);
  }

  m_avatar.theta = 0.0;
  m_avatar.height = DataStore::get<double>("AvatarHeight", 5.0);
  m_avatar.moon = m_moons.back();
  m_avatar.isJumping = false;

  m_avatar.up.ox = 0.0;
  m_avatar.up.oy = 0.0;
  m_avatar.up.nx = 0.0;
  m_avatar.up.ny = 1.0;
}

Universe::~Universe()
{
  BOOST_FOREACH(Moon* moon, m_moons)
  {
    delete moon;
  }
}

void Universe::moveLeft()
{
  m_avatar.theta += m_avatarAngularSpeed;
  if (m_avatar.theta > 2.0 * M_PI)
  {
    m_avatar.theta -= 2.0 * M_PI;
  }
}

void Universe::moveRight()
{
  m_avatar.theta -= m_avatarAngularSpeed;
  if (m_avatar.theta < 0.0)
  {
    m_avatar.theta += 2.0 * M_PI;
  }
}

const Avatar& Universe::getAvatar() const
{
  return m_avatar;
}

void Universe::jump()
{
  // calculate the direction of the avatar
  m_avatar.isJumping = true;

  // Calculate the world space co-ordinates of the ray
  assert(m_avatar.moon);

  const Moon& moon = *m_avatar.moon;
  const double angle = moon.theta + m_avatar.theta + M_PI / 2.0;
  m_avatar.up.ox = moon.x + moon.r * cos(angle);
  m_avatar.up.oy = moon.y + moon.r * sin(angle);
  m_avatar.up.nx = cos(angle);
  m_avatar.up.ny = sin(angle);

  Intersection result;
  result.t = 0.0;
  Moon* target = NULL;
  for (MoonList::iterator i = m_moons.begin(); i != m_moons.end(); ++i)
  {
    Intersection current;
    current.t = 0.0;
    if (intersectionRayMoon(m_avatar.up, **i, current))
    {
      if ((result.t == 0.0) || (current.t > 0.0 && current.t < result.t))
      {
        result = current;
        target = *i;
      }
    }
  }

  // If the ray intersects with a moon, move the avatar to it
  if (result.t > 0.0)
  {
    m_avatar.moon = target;

    const double rx = (result.x - target->x)/target->r;
    const double ry = (result.y - target->y)/target->r;

    const double theta = atan2(ry,rx);

    m_avatar.theta = wrap(0.0, 2.0*M_PI, theta - target->theta - M_PI / 2.0);
  }
}

void Universe::idle()
{
  m_avatar.isJumping = false;
}

bool Universe::isJumping() const
{
  return m_avatar.isJumping;
}

bool Universe::isIdle() const
{
  return !m_avatar.isJumping;
}
    
void Universe::execute(MoonOperation& op)
{
  op.begin();
  BOOST_FOREACH(Moon* moon, m_moons)
  {
    op.execute(*moon);
  }
  op.end();
}

void Universe::execute(MoonConstOperation& op) const
{
  op.begin();
  BOOST_FOREACH(const Moon* moon, m_moons)
  {
    op.execute(*moon);
  }
  op.end();
}

void Universe::execute(AvatarConstOperation& op) const
{
  op.begin();
  op.execute(m_avatar);
  op.end();
}

void Universe::update(const UpdateContext& context)
{
  if (isJumping())
  {
    idle();
  }

  updateMoonPositions(context);
  updateAvatarPosition(context);
  resolveCollisions();
}
  
void Universe::resolveCollisions()
{
  std::set<Moon*> markedForDestruction;

  for (MoonList::iterator i = m_moons.begin(); i != m_moons.end(); ++i)
  {
    for (MoonList::iterator j = i; j != m_moons.end(); ++j)
    {
      if (i == j) continue;

      Moon& moonA = **i;
      Moon& moonB = **j;
      
      CollisionResolution resolution;
      elasticCollision(m_domain, moonA, moonB, resolution);

      if (resolution.type == CollisionResolution::COLLISION)
      {
        moonA.u += resolution.impulseA.x / moonA.m;
        moonA.v += resolution.impulseA.y / moonA.m;
        moonB.u -= resolution.impulseB.x / moonB.m;
        moonB.v -= resolution.impulseB.y / moonB.m;
   
        if (shouldDestroyMoon(moonA, resolution.impulseA))
        {
          markedForDestruction.insert(&moonA);
        }
   
        if (shouldDestroyMoon(moonB, resolution.impulseB))
        {
          markedForDestruction.insert(&moonB);
        }
      }
    }
  }

  // Iterate through the moons marked for destruction and destroy them
  for (std::set<Moon*>::const_iterator it = markedForDestruction.begin();
    it != markedForDestruction.end(); ++it)
  {
    destroyMoon(*it);
  }
}

bool Universe::shouldDestroyMoon(const Moon& moon, const Vector2d& impulse) const
{
  const double magnitude = sqrt(dot(impulse,impulse));
  return !isAvatarOnThisMoon(moon) && 
    (magnitude > 5.0 * moon.r * moon.m);
}

bool Universe::isAvatarOnThisMoon(const Moon& moon) const
{
  return (m_avatar.moon == &moon);
}

void Universe::destroyMoon(Moon* moon)
{
  static const double sqrt3 = sqrt(3.0);
  if (moon->r > sqrt3 * DataStore::get<double>("MinMoonRadius", 5.0))
  {
    Vector2d origin;
    origin.x = 0.0;
    origin.y = 0.0;

    {
      Moon* newmoon = new Moon;
      newmoon->x = moon->x;
      newmoon->y = moon->y;
      newmoon->r = moon->r / sqrt3;
      newmoon->theta = moon->theta;
      newmoon->dtheta = moon->dtheta;
      newmoon->m = moon->m / 3.0;

      Vector2d velocity;
      velocity.x = moon->u;
      velocity.y = moon->v;

      Vector2d newVelocity = rotate(30.0, origin, velocity); 
      newmoon->u = newVelocity.x;
      newmoon->v = newVelocity.y;

      m_moons.push_back(newmoon);
    }
    {
      Moon* newmoon = new Moon;
      newmoon->x = moon->x;
      newmoon->y = moon->y;
      newmoon->r = moon->r / sqrt3;
      newmoon->u = moon->u;
      newmoon->v = moon->v;
      newmoon->theta = moon->theta;
      newmoon->dtheta = moon->dtheta;
      newmoon->m = moon->m / 3.0;
      m_moons.push_back(newmoon);
    }
    {
      Moon* newmoon = new Moon;
      newmoon->x = moon->x;
      newmoon->y = moon->y;
      newmoon->r = moon->r / sqrt3;
      newmoon->theta = moon->theta;
      newmoon->dtheta = moon->dtheta;
      newmoon->m = moon->m / 3.0;

      Vector2d velocity;
      velocity.x = moon->u;
      velocity.y = moon->v;

      Vector2d newVelocity = rotate(-30.0, origin, velocity); 
      newmoon->u = newVelocity.x;
      newmoon->v = newVelocity.y;

      m_moons.push_back(newmoon);
    }
  }
  m_moons.erase(std::find(m_moons.begin(), m_moons.end(), moon));
  delete moon;
}

void Universe::updateMoonPositions(const UpdateContext& context)
{
  const double dt = 1.0 / context.frameRate;
  BOOST_FOREACH(Moon* moon, m_moons)
  {
    moon->x = m_domain.toX(moon->x + dt * moon->u);
    moon->y = m_domain.toY(moon->y + dt * moon->v);
    moon->theta += dt * moon->dtheta;
    while(moon->theta > 2.0 * M_PI)
    {
      moon->theta -= 2.0 * M_PI;
    }
  }
}

void Universe::updateAvatarPosition(const UpdateContext& context)
{
  static double lastJump = 0.0;

  if (context.keyLeft)
  {
    moveLeft();
  }
  else if (context.keyRight)
  {
    moveRight();
  }
  else if (context.keyUp)
  {
    if (context.currentTime > lastJump + 0.5)
    {
      jump();
      lastJump = context.currentTime;
    }
  }
}


