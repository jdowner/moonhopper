#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <vector>
#include "Moon.h"
#include "Avatar.h"
#include "Ray.h"

struct MoonOperation
{
  virtual ~MoonOperation(){}
  virtual void begin() {}
  virtual void execute(const Moon& moon) = 0;
  virtual void end() {}
};

struct AvatarOperation
{
  virtual ~AvatarOperation(){}
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

    const MoonList& getMoons() const;
    MoonList& getMoons();
    void destroyMoon(Moon* moon);

    const Avatar& getAvatar() const;

    void execute(MoonOperation& op) const;
    void execute(AvatarOperation& op) const;

    void moveLeft();
    void moveRight();
    void jump();
    void idle();

    bool isJumping() const;
    bool isIdle() const;

    const Ray& getRay() const;

  private:
    
    MoonList m_moons;
    Avatar m_avatar;
    const double m_avatarAngularSpeed;
    Ray m_ray;
    bool m_jumping;
};

#endif // RENDERERCONTEXT_H

