#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <list>
#include <boost/scoped_ptr.hpp>
#include "Moon.h"
#include "Avatar.h"
#include "Hook.h"
#include "Ray.h"
#include "PeriodicDomain.h"
#include "Vector2d.h"

struct MoonOperation
{
  virtual ~MoonOperation(){}
  virtual void begin() {}
  virtual void execute(Moon& moon) = 0;
  virtual void end() {}
};

struct MoonConstOperation
{
  virtual ~MoonConstOperation(){}
  virtual void begin() {}
  virtual void execute(const Moon& moon) = 0;
  virtual void end() {}
};

struct AvatarConstOperation
{
  virtual ~AvatarConstOperation(){}
  virtual void begin() {}
  virtual void execute(const Avatar& moon) = 0;
  virtual void end() {}
};

typedef std::list<Moon*> MoonList;

struct UpdateContext;
struct CollisionResolution;

/**
 *
 */
class Universe
{
  public:
    Universe();
    ~Universe();

    const Avatar& getAvatar() const;

    void execute(MoonOperation& op);
    void execute(MoonConstOperation& op) const;
    void execute(AvatarConstOperation& op) const;

    void update(const UpdateContext& context);

    void moveLeft();
    void moveRight();
    void jump();
    void idle();
    void launchHook();

    bool isJumping() const;
    bool isIdle() const;
    bool isAvatarOnThisMoon(const Moon& moon) const;
    bool isHookExtant() const;

    const Hook& getHook() const;

  private:
    void resolveCollisions();
    void resolveMoonCollisions();
    void resolveHookCollisions();

    void updatePositions(const UpdateContext& context);
    void updateMoonPositions(const UpdateContext& context);
    void updateAvatarPosition(const UpdateContext& context);
    void updateHookPosition(const UpdateContext& context);

    bool shouldDestroyMoon(const Moon& moon, const Vector2d& impulse) const;
    void destroyMoon(Moon* moon);

  private:
    PeriodicDomain m_domain;   
    MoonList m_moons;
    Avatar m_avatar;
    boost::scoped_ptr<Hook> m_hook;
    const double m_avatarAngularSpeed;
};

#endif // UNIVERSE_H

