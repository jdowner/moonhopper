#include "Engine.h"
#include <stdexcept>
#include <cassert>
#include <boost/foreach.hpp>
#include <GL/glfw.h>
#include <GL/glu.h>
#include "DataStore.h"

Engine::Engine() : 
  m_running(false), 
  m_frameRate(30.0), 
  m_lastUpdate(0),
  m_domain(
    DataStore::get<double>("DomainMinX", -300.0),
    DataStore::get<double>("DomainMaxX", 300.0),
    DataStore::get<double>("DomainMinY", -300.0),
    DataStore::get<double>("DomainMaxY", 300.0)),
  m_spatial(
    m_domain,
    DataStore::get<double>("MoonRadius", 10.0))
{
}

void Engine::init()
{
  if (!glfwInit())
  {
    throw std::runtime_error("Unable to initialize glfw");
  }

  if (!glfwOpenWindow(600,600,0,0,0,0,0,0,GLFW_WINDOW))
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
  m_lastUpdate = glfwGetTime();

  updateMoonPositions();
  updateSpatialTree();
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
  }
}
  
void Engine::updateSpatialTree()
{
  m_spatial.clear();

  BOOST_FOREACH(Moon& moon, m_context.getMoons())
  {
    m_spatial.add(&moon);
  }
}

