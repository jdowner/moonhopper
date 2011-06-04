#include "RendererContext.h"
#include <cstdlib>
#include <cmath>
#include "DataStore.h"

namespace
{
  double uniform()
  {
    return double(rand()) / RAND_MAX;
  }
}

RendererContext::RendererContext()
: m_avatarAngularSpeed(DataStore::get<double>("AvatarAngularSpeed", 0.05))
{
  // For now we are just generating some random positions for th moons
  
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
}

void RendererContext::moveRight()
{
  m_avatar.theta -= m_avatarAngularSpeed;
}

const Avatar& RendererContext::getAvatar() const
{
  return m_avatar;
}
