#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <vector>
#include "Moon.h"
#include "Avatar.h"

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

  private:
    std::vector<Moon> m_moons;
    Avatar m_avatar;
    const double m_avatarAngularSpeed;
};

#endif // RENDERERCONTEXT_H

