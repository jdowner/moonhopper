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
#include "TetheredMoons.h"

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

    const MoonList& getMoons() const;
    const Avatar& getAvatar() const;
    const Hook& getHook() const;

    void update(const UpdateContext& context);

    void moveLeft();
    void moveRight();
    void jump();
    void idle();
    void launchHook();

    bool isJumping() const;
    bool isIdle() const;
    bool isAvatarOnThisMoon(const Moon& moon) const;
    bool isAvatarDead() const;
    bool isHookExtant() const;
    bool isTetherExtant() const;
    bool isTethered(const Moon* moon) const;

  private:
    void resolveCollisions();
    void resolveMoonCollisions();
    void resolveHookCollisions();
    void resolveTetherCollisions();

    void updatePositions(const UpdateContext& context);
    void updateMoonPositions(const UpdateContext& context);
    void updateAvatarPosition(const UpdateContext& context);
    void updateHookPosition(const UpdateContext& context);
    void updateTetherPosition(const UpdateContext& context);

    bool shouldDestroyMoon(const Moon& moon, const Vector2d& impulse) const;
    void destroyMoon(Moon* moon);

    void setTetheredMoons(Moon* moonA, Moon* moonB);
    void releaseTetheredMoons();

  private:
    PeriodicDomain m_domain;   
    MoonList m_moons;
    Avatar m_avatar;
    boost::scoped_ptr<Hook> m_hook;
    const double m_avatarAngularSpeed;

    TetheredMoons m_tether;
};

#endif // UNIVERSE_H

