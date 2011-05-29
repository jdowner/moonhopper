#include "RendererContext.h"
#include <cstdlib>
#include <cmath>

namespace
{
  double uniform()
  {
    return double(rand()) / RAND_MAX;
  }
}

RendererContext::RendererContext()
{
  // For now we are just generating some random positions for th moons
  
  const unsigned int NUM_MOONS = 12;
  const double MIN_X = -300.0;
  const double MAX_X = 300.0;
  const double MIN_Y = -300.0;
  const double MAX_Y = 300.0;

  for (unsigned int i = 0; i < NUM_MOONS; ++i)
  {
    Moon moon;
    moon.r = 10.0;
    moon.x = MIN_X + (MAX_X - MIN_X) * uniform();
    moon.y = MIN_Y + (MAX_Y - MIN_Y) * uniform();
    moon.theta = 2.0 * M_PI * uniform();

    m_moons.push_back(moon);
  }
}

const std::vector<Moon>& RendererContext::getMoons() const
{
  return m_moons;
}
