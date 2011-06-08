#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <vector>
#include "Moon.h"
#include "Avatar.h"
#include "Ray.h"

/**
 *
 */
class RendererContext
{
  public:
    RendererContext();

    const std::vector<Moon>& getMoons() const;
    std::vector<Moon>& getMoons();
    const Avatar& getAvatar() const;

    void moveLeft();
    void moveRight();
    void jump();
    void idle();

    bool isJumping() const;
    bool isIdle() const;

    const Ray& getRay() const;

  private:
    std::vector<Moon> m_moons;
    Avatar m_avatar;
    const double m_avatarAngularSpeed;
    Ray m_ray;
    bool m_jumping;
};

#endif // RENDERERCONTEXT_H

