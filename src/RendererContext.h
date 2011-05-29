#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <vector>
#include "Moon.h"

/**
 *
 */
class RendererContext
{
  public:
    RendererContext();

    const std::vector<Moon>& getMoons() const;
    std::vector<Moon>& getMoons();

  private:
    std::vector<Moon> m_moons;
};

#endif // RENDERERCONTEXT_H

