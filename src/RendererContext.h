#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <vector>
#include "Moon.h"
#include "Avatar.h"
#include "Ray.h"
#include "PeriodicDomain.h"

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

/**
 *
 */
class RendererContext
{
  public:
    RendererContext();

    void destroyMoon(Moon* moon);

    const Avatar& getAvatar() const;

    void execute(MoonOperation& op);
    void execute(MoonConstOperation& op) const;
    void execute(AvatarConstOperation& op) const;

    void update(double dt);

    void moveLeft();
    void moveRight();
    void jump();
    void idle();

    bool isJumping() const;
    bool isIdle() const;

    const Ray& getRay() const;

  private:
    void resolveCollisions();
    void updateMoonPositions(double dt);

  private:
    PeriodicDomain m_domain;   
    MoonList m_moons;
    Avatar m_avatar;
    const double m_avatarAngularSpeed;
    Ray m_ray;
    bool m_jumping;
};

#endif // RENDERERCONTEXT_H

