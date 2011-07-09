#include "Engine.h"
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <cmath>
#include <boost/foreach.hpp>
#include <GL/glfw.h>
#include <GL/glu.h>
#include "DataStore.h"
#include "CollisionResolution.h"
#include "MathUtils.h"
#include "UpdateContext.h"

Engine::Engine() : 
  m_running(false), 
  m_frameRate(30.0), 
  m_lastUpdate(0)
{
}

void Engine::init()
{
  if (!glfwInit())
  {
    throw std::runtime_error("Unable to initialize glfw");
  }

  const double height = DataStore::get<double>("WindowHeight", 800.0);
  const double width = DataStore::get<double>("WindowWidth", 1000.0);
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
  m_renderer.render(m_universe);
}

void Engine::update()
{
  m_running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

  UpdateContext context = createUpdateContext();
  m_universe.update(context);
  
  m_lastUpdate = context.currentTime;
}
    
UpdateContext Engine::createUpdateContext() const
{
  UpdateContext context;
  context.currentTime = glfwGetTime();
  context.frameRate = m_frameRate;
  context.keyLeft = (GLFW_PRESS == glfwGetKey(GLFW_KEY_LEFT));
  context.keyRight = (GLFW_PRESS == glfwGetKey(GLFW_KEY_RIGHT));
  context.keyUp = (GLFW_PRESS == glfwGetKey(GLFW_KEY_UP));
  context.keyH = (GLFW_PRESS == glfwGetKey(72));
  context.keyG = (GLFW_PRESS == glfwGetKey(71));
  context.keyTab = (GLFW_PRESS == glfwGetKey(GLFW_KEY_TAB));

  return context;
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

