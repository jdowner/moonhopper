#include "RendererContext.h"
#include <cstdlib>
#include <cmath>
#include <cassert>
#include "DataStore.h"
#include "Ray.h"
#include "CollisionDetection.h"

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

RendererContext::RendererContext()
: m_avatarAngularSpeed(DataStore::get<double>("AvatarAngularSpeed", 0.05))
, m_jumping(false)
{
  // For now we are just generating some random positions for the moons
  
  const unsigned int NUM_MOONS = DataStore::get<int>("NumberOfMoons", 32);
  const double MIN_X = DataStore::get<double>("DomainMinX", -300.0);
  const double MAX_X = DataStore::get<double>("DomainMaxX", 300.0);
  const double MIN_Y = DataStore::get<double>("DomainMinY", -300.0);
  const double MAX_Y = DataStore::get<double>("DomainMaxY", 300.0);
  const double MAX_VELOCITY = DataStore::get<double>("MaxVelocity", 30.0);
  const double MAX_ANGULARVELOCITY = 
    DataStore::get<double>("MaxAngularVelocity", M_PI);
  const double MIN_RADIUS = DataStore::get<double>("MinMoonRadius", 5.0);
  const double MAX_RADIUS = DataStore::get<double>("MaxMoonRadius", 30.0);

  for (unsigned int i = 0; i < NUM_MOONS; ++i)
  {
    Moon moon;
    moon.r = MIN_RADIUS + (MAX_RADIUS - MIN_RADIUS) * uniform();
    moon.x = MIN_X + (MAX_X - MIN_X) * uniform();
    moon.y = MIN_Y + (MAX_Y - MIN_Y) * uniform();
    moon.theta = 2.0 * M_PI * uniform();
    moon.dtheta = M_PI * uniform();

    const double vAngle = 2.0 * M_PI * uniform();
    const double vMagnitude = MAX_VELOCITY * uniform();
    moon.u = vMagnitude * cos(vAngle);
    moon.v = vMagnitude * sin(vAngle);

    m_moons.push_back(moon);
  }

  m_avatar.theta = 0.0;
  m_avatar.height = DataStore::get<double>("AvatarHeight", 5.0);
  m_avatar.moon = &m_moons.back();

  m_ray.ox = 0.0;
  m_ray.oy = 0.0;
  m_ray.nx = 0.0;
  m_ray.ny = 1.0;
}

const std::vector<Moon>& RendererContext::getMoons() const
{
  return m_moons;
}

std::vector<Moon>& RendererContext::getMoons()
{
  return m_moons;
}

void RendererContext::moveLeft()
{
  m_avatar.theta += m_avatarAngularSpeed;
  if (m_avatar.theta > 2.0 * M_PI)
  {
    m_avatar.theta -= 2.0 * M_PI;
  }
}

void RendererContext::moveRight()
{
  m_avatar.theta -= m_avatarAngularSpeed;
  if (m_avatar.theta < 0.0)
  {
    m_avatar.theta += 2.0 * M_PI;
  }
}

const Avatar& RendererContext::getAvatar() const
{
  return m_avatar;
}

void RendererContext::jump()
{
  // calculate the direction of the avatar
  m_jumping = true;

  // Calculate the world space co-ordinates of the ray
  assert(m_avatar.moon);

  const Moon& moon = *m_avatar.moon;
  const double angle = moon.theta + m_avatar.theta + M_PI / 2.0;
  m_ray.ox = moon.x + moon.r * cos(angle);
  m_ray.oy = moon.y + moon.r * sin(angle);
  m_ray.nx = cos(angle);
  m_ray.ny = sin(angle);

  Intersection result;
  result.t = 0.0;
  size_t index = 0;
  for (size_t i = 0; i < m_moons.size(); ++i)
  {
    Intersection current;
    current.t = 0.0;
    if (intersectionRayMoon(m_ray, m_moons[i], current))
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

void RendererContext::idle()
{
  m_jumping = false;
}

bool RendererContext::isJumping() const
{
  return m_jumping;
}

bool RendererContext::isIdle() const
{
  return !m_jumping;
}

const Ray& RendererContext::getRay() const
{
  return m_ray;
}
