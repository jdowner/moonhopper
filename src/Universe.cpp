#include "Universe.h"
#include <cstdlib>
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
    Moon moon;
    moon.r = MIN_RADIUS + (MAX_RADIUS - MIN_RADIUS) * uniform();
    moon.m = M_PI * moon.r * moon.r;
    moon.x = MIN_X + (MAX_X - MIN_X) * uniform();
    moon.y = MIN_Y + (MAX_Y - MIN_Y) * uniform();
    moon.theta = 2.0 * M_PI * uniform();
    moon.dtheta = MIN_ANGULAR_VELOCITY + 
      (MAX_ANGULAR_VELOCITY - MIN_ANGULAR_VELOCITY) * uniform();

    const double vAngle = 2.0 * M_PI * uniform();
    const double vMagnitude = MAX_VELOCITY * uniform();
    moon.u = vMagnitude * cos(vAngle);
    moon.v = vMagnitude * sin(vAngle);

    m_moons.push_back(moon);
  }

  m_avatar.theta = 0.0;
  m_avatar.height = DataStore::get<double>("AvatarHeight", 5.0);
  m_avatar.moon = &m_moons.back();
  m_avatar.isJumping = false;

  m_avatar.up.ox = 0.0;
  m_avatar.up.oy = 0.0;
  m_avatar.up.nx = 0.0;
  m_avatar.up.ny = 1.0;
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
  size_t index = 0;
  for (size_t i = 0; i < m_moons.size(); ++i)
  {
    Intersection current;
    current.t = 0.0;
    if (intersectionRayMoon(m_avatar.up, m_moons[i], current))
    {
      if ((result.t == 0.0) || (current.t > 0.0 && current.t < result.t))
      {
        result = current;
        index = i;
      }
    }
  }

  // If the ray intersects with a moon, move the avatar to it
  if (result.t > 0.0)
  {
    Moon& moon = m_moons[index];
    m_avatar.moon = &moon;

    const double rx = (result.x - moon.x)/moon.r;
    const double ry = (result.y - moon.y)/moon.r;

    const double theta = atan2(ry,rx);

    m_avatar.theta = wrap(0.0, 2.0*M_PI, theta - moon.theta - M_PI / 2.0);
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
  BOOST_FOREACH(Moon& moon, m_moons)
  {
    op.execute(moon);
  }
  op.end();
}

void Universe::execute(MoonConstOperation& op) const
{
  op.begin();
  BOOST_FOREACH(const Moon& moon, m_moons)
  {
    op.execute(moon);
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
  std::set<size_t> markedForDestruction;

  for(size_t i = 0; i < m_moons.size() - 1; ++i)
  {
    for(size_t j = i + 1; j < m_moons.size(); ++j)
    {
      CollisionResolution resolution;
      elasticCollision(m_domain, m_moons[i], m_moons[j], resolution);


      if (resolution.type == CollisionResolution::COLLISION)
      {
        m_moons[i].u += resolution.impulseA.x / m_moons[i].m;
        m_moons[i].v += resolution.impulseA.y / m_moons[i].m;
        m_moons[j].u -= resolution.impulseB.x / m_moons[j].m;
        m_moons[j].v -= resolution.impulseB.y / m_moons[j].m;
   
        if (shouldDestroyMoon(i, resolution.impulseA))
        {
          markedForDestruction.insert(i);
        }
   
        if (shouldDestroyMoon(j, resolution.impulseB))
        {
          markedForDestruction.insert(j);
        }
      }
    }
  }

  // Iterate through the moons marked for destruction and destroy them
  for (std::set<size_t>::const_iterator it = markedForDestruction.begin();
    it != markedForDestruction.end(); ++it)
  {
    destroyMoon(*it);
  }
}

bool Universe::shouldDestroyMoon(size_t i, const Vector2d& impulse) const
{
  const double magnitude = sqrt(dot(impulse,impulse));
  return (magnitude > 2.0 * m_moons[i].r * m_moons[i].m);
}

void Universe::destroyMoon(size_t index)
{
  std::swap(m_moons[index], m_moons[m_moons.size() - 1]);
  m_moons.resize(m_moons.size() - 1);
}

void Universe::updateMoonPositions(const UpdateContext& context)
{
  const double dt = 1.0 / context.frameRate;
  BOOST_FOREACH(Moon& moon, m_moons)
  {
    moon.x = m_domain.toX(moon.x + dt * moon.u);
    moon.y = m_domain.toY(moon.y + dt * moon.v);
    moon.theta += dt * moon.dtheta;
    while(moon.theta > 2.0 * M_PI)
    {
      moon.theta -= 2.0 * M_PI;
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


