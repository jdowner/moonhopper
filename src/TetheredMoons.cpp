#include "TetheredMoons.h"
#include <cassert>
#include <cmath>
#include "Moon.h"

namespace
{
  Vector2d momentum(const Moon* moon)
  {
    assert(moon);

    Vector2d p;

    p.x = moon->m * moon->u;
    p.y = moon->m * moon->v;

    return p;
  }

  double distanceSqr(double xa, double ya, double xb, double yb)
  {
    const double dx = xa - xb;
    const double dy = ya - yb;
    return (dx * dx + dy * dy);
  }
}

TetheredMoons::TetheredMoons()
{
  reset();
}

void TetheredMoons::reset()
{
  m_moonA = 0;
  m_moonB = 0;
  m_velocity.x = 0.0;
  m_velocity.y = 0.0;
  m_position.x = 0.0;
  m_position.y = 0.0;
  m_theta = 0.0;
  m_omega = 0.0;
}

void TetheredMoons::setMoons(Moon* moonA, Moon* moonB)
{
  assert(moonA);
  assert(moonB);

  m_moonA = moonA;
  m_moonB = moonB;

  m_totalMass = m_moonA->m + m_moonB->m;

  // For now, the existing rotation of the moons will not affect the tether
  m_moonA->dtheta = 0.0;
  m_moonB->dtheta = 0.0;

  calculateCOM();
  calculateInertia();

  applyImpulse(moonA->x, moonA->y, momentum(moonA));
  applyImpulse(moonB->x, moonB->y, momentum(moonB));
}

Moon* TetheredMoons::moonA()
{
  return m_moonA;
}

Moon* TetheredMoons::moonB()
{
  return m_moonB;
}

const Moon* TetheredMoons::moonA() const
{
  return m_moonA;
}

const Moon* TetheredMoons::moonB() const
{
  return m_moonB;
}

bool TetheredMoons::isExtant() const
{
  // If moon A is not null than moon B is not null too
  return (0 != m_moonA);
}

const Vector2d& TetheredMoons::position() const
{
  return m_position;
}

const Vector2d& TetheredMoons::velocity() const
{
  return m_velocity;
}

double TetheredMoons::angle() const
{
  return m_theta;
}

double TetheredMoons::angularVelocity() const
{
  return m_omega;
}

void TetheredMoons::moveBy(const Vector2d& delta)
{
  m_position.x += delta.x;
  m_position.y += delta.y;
  m_moonA->x += delta.x;
  m_moonB->x += delta.x;
  m_moonA->y += delta.y;
  m_moonB->y += delta.y;
}

void TetheredMoons::setPosition(const Vector2d& position)
{
  m_position = position;
}

void TetheredMoons::setPosition(double x, double y)
{
  m_position.x = x;
  m_position.y = y;
}

void TetheredMoons::setVelocity(const Vector2d& velocity)
{
  m_velocity = velocity;
}

void TetheredMoons::setVelocity(double u, double v)
{
  m_velocity.x = u;
  m_velocity.y = v;
}

void TetheredMoons::setAngle(double theta)
{
  m_theta = theta;
}

void TetheredMoons::setAngularVelocity(double omega)
{
  m_omega = omega;
}

void TetheredMoons::applyImpulse(const Vector2d& location, const Vector2d& impulse)
{
  applyImpulse(location.x, location.y, impulse);
}

void TetheredMoons::applyImpulse(double x, double y, const Vector2d& impulse)
{
  // Apply impulse at center of mass
  m_velocity.x += impulse.x / m_totalMass;
  m_velocity.y += impulse.y / m_totalMass;

  // Calculate the moment about the COM
  const double dx = x - m_position.x;
  const double dy = y - m_position.y;
  const double moment = dx * impulse.y - dy * impulse.x;

  // Calculate the angular velocity
  m_omega += moment / m_inertia;
}

void TetheredMoons::calculateInertia()
{
  const double IzA = m_moonA->m * m_moonA->r * m_moonA->r / 2.0;
  const double IzB = m_moonB->m * m_moonB->r * m_moonB->r / 2.0;

  const double dA2 = distanceSqr(m_moonA->x, m_moonA->y, m_position.x, m_position.y);
  const double dB2 = distanceSqr(m_moonB->x, m_moonB->y, m_position.x, m_position.y);

  m_inertia = IzA + m_moonA->m * dA2 + IzB + m_moonB->m * dB2;
}
  
void TetheredMoons::calculateCOM()
{
  assert(m_moonA);
  assert(m_moonB);

  m_position.x = (m_moonA->m * m_moonA->x + m_moonB->m * m_moonB->x) / m_totalMass;
  m_position.y = (m_moonA->m * m_moonA->y + m_moonB->m * m_moonB->y) / m_totalMass;
}

