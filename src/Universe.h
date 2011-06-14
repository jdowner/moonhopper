#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <vector>
#include "Moon.h"
#include "Avatar.h"
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

typedef std::vector<Moon> MoonList;

struct UpdateContext;
struct CollisionResolution;

/**
 *
 */
class Universe
{
  public:
    Universe();

    const Avatar& getAvatar() const;

    void execute(MoonOperation& op);
    void execute(MoonConstOperation& op) const;
    void execute(AvatarConstOperation& op) const;

    void update(const UpdateContext& context);

    void moveLeft();
    void moveRight();
    void jump();
    void idle();

    bool isJumping() const;
    bool isIdle() const;

  private:
    void resolveCollisions();
    void updateMoonPositions(const UpdateContext& context);
    void updateAvatarPosition(const UpdateContext& context);
    bool shouldDestroyMoon(size_t i, const Vector2d& impulse) const;
    void destroyMoon(size_t index);

  private:
    PeriodicDomain m_domain;   
    MoonList m_moons;
    Avatar m_avatar;
    const double m_avatarAngularSpeed;
};

#endif // UNIVERSE_H

