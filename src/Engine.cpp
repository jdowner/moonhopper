#include "Engine.h"
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <boost/foreach.hpp>
#include <GL/glfw.h>
#include <GL/glu.h>
#include "DataStore.h"
#include "CollisionResolution.h"

Engine::Engine() : 
  m_running(false), 
  m_frameRate(30.0), 
  m_lastUpdate(0),
  m_domain(
    DataStore::get<double>("DomainMinX", -300.0),
    DataStore::get<double>("DomainMaxX", 300.0),
    DataStore::get<double>("DomainMinY", -300.0),
    DataStore::get<double>("DomainMaxY", 300.0))
{
}

void Engine::init()
{
  if (!glfwInit())
  {
    throw std::runtime_error("Unable to initialize glfw");
  }

  const double height = m_domain.getHeight();
  const double width = m_domain.getWidth();
  if (!glfwOpenWindow(width,height,0,0,0,0,0,0,GLFW_WINDOW))
  {
    glfwTerminate();
    throw std::runtime_error("Unable to create window");
  }

  m_renderer.init();

  m_running = true;
}

void Engine::shutdown()
{
  glfwTerminate();
}

bool Engine::running() const
{
  return m_running;
}

void Engine::render()
{ 
  m_renderer.render(m_context);

  glfwSleep(0.01);
}

void Engine::update()
{
  m_running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

  if (m_context.isJumping())
  {
    m_context.idle();
  }

  updateMoonPositions();
  updateAvatarPosition();
  resolveCollisions();
  
  m_lastUpdate = glfwGetTime();
}

void Engine::sleep()
{
  double elapsed = glfwGetTime() - m_lastUpdate;
  const double period = 1.0 / m_frameRate;
  while (elapsed < period)
  {
    glfwSleep(period - elapsed);
    elapsed = glfwGetTime() - m_lastUpdate;
  }
}
    
void Engine::updateMoonPositions()
{
  const double dt = 1.0 / m_frameRate;
  BOOST_FOREACH(Moon& moon, m_context.getMoons())
  {
    moon.x = m_domain.toX(moon.x + dt * moon.u);
    moon.y = m_domain.toY(moon.y + dt * moon.v);
    moon.theta += dt * moon.dtheta;
    while(moon.theta > 2.0 * M_PI)
    {
      moon.theta -= 2.0 * M_PI;
    }
  }
}

void Engine::updateAvatarPosition()
{
  static double lastJump = 0.0;

  if (GLFW_PRESS == glfwGetKey(GLFW_KEY_LEFT))
  {
    m_context.moveLeft();
  }
  else if (GLFW_PRESS == glfwGetKey(GLFW_KEY_RIGHT))
  {
    m_context.moveRight();
  }
  else if (GLFW_PRESS == glfwGetKey(GLFW_KEY_UP))
  {
    if (glfwGetTime() > lastJump + 0.5)
    {
      m_context.jump();
      lastJump = glfwGetTime();
    }
  }
}
  
void Engine::resolveCollisions()
{
  std::vector<Moon>& moons = m_context.getMoons();
  for(size_t i = 0; i < moons.size() - 1; ++i)
  {
    for(size_t j = i + 1; j < moons.size(); ++j)
    {
      elasticCollision(m_domain, moons[i], moons[j]);
    }
  }
}
