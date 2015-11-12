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
  m_avatar.isDead = false;

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

const MoonList& Universe::getMoons() const
{
  return m_moons;
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

bool Universe::isHookExtant() const
{
  return m_hook != 0;
}

bool Universe::isTetherExtant() const
{
  return m_tether.isExtant();
}

void Universe::update(const UpdateContext& context)
{
  if (isJumping())
  {
    idle();
  }

  if (context.keyG && isTetherExtant())
  {
    releaseTetheredMoons();
  }

  updatePositions(context);
  resolveCollisions();
}

void Universe::updatePositions(const UpdateContext& context)
{
  updateMoonPositions(context);
  updateAvatarPosition(context);
  updateHookPosition(context);
  updateTetherPosition(context);
}
  
void Universe::resolveCollisions()
{
  resolveMoonCollisions();
  resolveHookCollisions();
  resolveTetherCollisions();
}

void Universe::resolveHookCollisions()
{
  if (isHookExtant())
  {
    for (MoonList::const_iterator i = m_moons.begin(); i != m_moons.end(); ++i)
    {
      if ((*i) == m_avatar.moon)
      {
        continue;
      }

      const double r = (*i)->r + m_hook->radius;
      const double dx = m_hook->position.x - (*i)->x;
      const double dy = m_hook->position.y - (*i)->y;
      if (dx * dx + dy * dy < r * r)
      {
        setTetheredMoons(m_avatar.moon, *i);
        m_hook.reset();
        break;
      }
    }
  }
}

void Universe::resolveMoonCollisions()
{
  std::set<Moon*> markedForDestruction;

  for (MoonList::iterator i = m_moons.begin(); i != m_moons.end(); ++i)
  {
    if (isTethered(*i)) continue;

    Moon& moonA = **i;

    for (MoonList::iterator j = i; j != m_moons.end(); ++j)
    {
      if (i == j) continue;
      if (isTethered(*j)) continue;

      Moon& moonB = **j;
      
      CollisionResolution resolution;
      elasticCollision(m_domain, moonA, moonB, resolution);

      if (resolution.type == CollisionResolution::COLLISION)
      {
        moonA.u += resolution.impulseA.x / moonA.m;
        moonA.v += resolution.impulseA.y / moonA.m;
        moonB.u -= resolution.impulseB.x / moonB.m;
        moonB.v -= resolution.impulseB.y / moonB.m;

        moonA.dtheta += resolution.angularImpulseA / (moonA.m * moonA.r * moonA.r);
        moonB.dtheta -= resolution.angularImpulseB / (moonB.m * moonB.r * moonB.r);

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

void Universe::resolveTetherCollisions()
{
  if (isTetherExtant())
  {
    std::set<Moon*> markedForDestruction;

    // Check for collisions with tethered moons
    BOOST_FOREACH(Moon* moon, m_moons)
    {
      if (isTethered(moon)) continue;

      CollisionResolution resolution;
      
      elasticCollision(m_domain, *m_tether.moonA(), *moon, resolution);
      if (resolution.type == CollisionResolution::COLLISION)
      {
        m_tether.applyImpulse(resolution.location, resolution.impulseA);
        moon->u -= resolution.impulseB.x / moon->m;
        moon->v -= resolution.impulseB.y / moon->m;

        if (shouldDestroyMoon(*moon, resolution.impulseB))
        {
          markedForDestruction.insert(moon);
        }
      }
      
      elasticCollision(m_domain, *m_tether.moonB(), *moon, resolution);

      if (resolution.type == CollisionResolution::COLLISION)
      {
        m_tether.applyImpulse(resolution.location, resolution.impulseA);
        moon->u -= resolution.impulseB.x / moon->m;
        moon->v -= resolution.impulseB.y / moon->m;

        if (shouldDestroyMoon(*moon, resolution.impulseB))
        {
          markedForDestruction.insert(moon);
        }
      }
    } 

    // TODO Check for collisions with the tether itself

    // Iterate through the moons marked for destruction and destroy them
    for (std::set<Moon*>::const_iterator it = markedForDestruction.begin();
        it != markedForDestruction.end(); ++it)
    {
      destroyMoon(*it);
    }
  }
}

bool Universe::shouldDestroyMoon(const Moon& moon, const Vector2d& impulse) const
{
  const double magnitude = sqrt(dot(impulse,impulse));
  return !isAvatarOnThisMoon(moon) && 
    (magnitude > DataStore::get<double>("DestructionSpeed", 2.0) * moon.r * moon.m);
}

bool Universe::isAvatarOnThisMoon(const Moon& moon) const
{
  return (m_avatar.moon == &moon);
}

bool Universe::isAvatarDead() const
{
  return m_avatar.isDead;
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
    if (isTethered(moon)) continue;

    moon->x = m_domain.toX(moon->x + dt * moon->u);
    moon->y = m_domain.toY(moon->y + dt * moon->v);
    moon->theta += dt * moon->dtheta;
    while(moon->theta > 2.0 * M_PI)
    {
      moon->theta -= 2.0 * M_PI;
    }
  }
}

void Universe::updateTetherPosition(const UpdateContext& context)
{
  if (isTetherExtant())
  {
    const double dt = 1.0 / context.frameRate;

    // TODO get the tether to work properly across the periodic boundaries

    // Update the rotation of the tether
    const double theta = m_tether.angle();
    const double omega = m_tether.angularVelocity();
    
    m_tether.setAngle(theta + dt * omega);

    // Update the position of the tether
    const Vector2d velocity = m_tether.velocity();

    Vector2d delta; 
    delta.x = dt * velocity.x;
    delta.y = dt * velocity.y;

    m_tether.moveBy(delta);

    // Now we need to synchronize the kinematics of the moons.  The translation
    // of the tether has already been accounted for above through the moveBy()
    // call. Now, we have to account for the rotation of the tether and its
    // effect on the position and velocity of the tethered moons

    Moon* moonA = m_tether.moonA();
    Moon* moonB = m_tether.moonB();
 
    // Determine the change in the angle of rotation and get the tethers
    // center-of-mass
    const double dtheta = dt * omega;
    const double c = cos(dtheta);
    const double s = sin(dtheta);
   
    const Vector2d com = m_tether.position();

    // Update moon A
    double dx = moonA->x - com.x;
    double dy = moonA->y - com.y;

    moonA->x = c * dx - s * dy + com.x; 
    moonA->y = s * dx + c * dy + com.y;

    moonA->u = velocity.x - omega * (s * dx + c * dy);
    moonA->v = velocity.y + omega * (c * dx - s * dy);

    // Update moon B
    dx = moonB->x - com.x;
    dy = moonB->y - com.y;

    moonB->x = c * dx - s * dy + com.x; 
    moonB->y = s * dx + c * dy + com.y; 

    moonB->u = velocity.x - omega * (s * dx + c * dy);
    moonB->v = velocity.y + omega * (c * dx - s * dy);
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

void Universe::updateHookPosition(const UpdateContext& context)
{
  if (not isHookExtant())
  {
    if (context.keyH && !isTetherExtant())
    {
      launchHook();
    }
  }
  else
  {
    const double dt = 1.0 / context.frameRate;
    m_hook->position.x += dt * m_hook->velocity.x;
    m_hook->position.y += dt * m_hook->velocity.y;
    m_hook->position.x = m_domain.toX(m_hook->position.x);
    m_hook->position.y = m_domain.toY(m_hook->position.y);
  }
}

void Universe::launchHook()
{
  assert(!isHookExtant());
  assert(!isJumping());

  const Moon& moon = *m_avatar.moon;
  const double angle = moon.theta + m_avatar.theta + M_PI / 2.0;
  const double speed = DataStore::get<double>("HookSpeed",10.0);

  m_hook.reset(new Hook);
  m_hook->position.x = moon.x + moon.r * cos(angle);
  m_hook->position.y = moon.y + moon.r * sin(angle);
  m_hook->velocity.x = speed * cos(angle);
  m_hook->velocity.y = speed * sin(angle);
  m_hook->angle = angle - M_PI / 2.0;
  m_hook->radius = DataStore::get<double>("HookRadius",20.0);
}

const Hook& Universe::getHook() const
{
  return *m_hook;
}
    
void Universe::setTetheredMoons(Moon* moonA, Moon* moonB)
{
  m_tether.setMoons(moonA, moonB);
}

void Universe::releaseTetheredMoons()
{
  m_tether.reset();
}
    
bool Universe::isTethered(const Moon* moon) const
{
  return moon && ((moon == m_tether.moonA()) || (moon == m_tether.moonB()));
}

