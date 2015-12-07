#include "Engine.h"
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <cmath>
#include <boost/foreach.hpp>
#include <GL/glu.h>
#include "DataStore.h"
#include "GLFW.h"
#include "CollisionResolution.h"
#include "MathUtils.h"
#include "UpdateContext.h"

#include <iostream>

Engine::Engine() :
  m_running(false),
  m_frameRate(30.0),
  m_lastUpdate(0),
  m_window(NULL)
{
}


#if USE_GLFW3
#include <GLFW/glfw3.h>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  std::cout << key << std::endl;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
#endif // USE_GLFW3

void Engine::init()
{
  glfw::init();

  const double height = DataStore::get<double>("WindowHeight", 800.0);
  const double width = DataStore::get<double>("WindowWidth", 1000.0);

  m_window = new glfw::Window(width, height, "Moonhopper");
  m_window->makeCurrent();

//  glfwSetKeyCallback(m_window->m_window, key_callback);

  m_renderer.init();

  m_running = true;
}

void Engine::shutdown()
{
  glfw::terminate();
}

bool Engine::running() const
{
  return m_running;
}

void Engine::render()
{
  m_renderer.render(m_universe);
  m_window->swapBuffers();
}


void Engine::update()
{
  m_window->pollEvents();

  m_running = !glfw::getKey(glfw::KEY_ESC) && m_window->isOpen();

  UpdateContext context = createUpdateContext();
  m_universe.update(context);

  m_lastUpdate = context.currentTime;
}

UpdateContext Engine::createUpdateContext() const
{
  UpdateContext context;
  context.currentTime = glfw::time();
  context.frameRate = m_frameRate;
  context.keyLeft = (glfw::KEY_PRESS == glfw::getKey(glfw::KEY_LEFT));
  context.keyRight = (glfw::KEY_PRESS == glfw::getKey(glfw::KEY_RIGHT));
  context.keyUp = (glfw::KEY_PRESS == glfw::getKey(glfw::KEY_UP));
  context.keyG = (glfw::KEY_PRESS == glfw::getKey(glfw::KEY_G));
  context.keyH = (glfw::KEY_PRESS == glfw::getKey(glfw::KEY_H));
  context.keyTab = (glfw::KEY_PRESS == glfw::getKey(glfw::KEY_TAB));

  return context;
}

void Engine::sleep()
{
  double elapsed = glfw::time() - m_lastUpdate;
  const double period = 1.0 / m_frameRate;
  while (elapsed < period)
  {
    glfw::sleep(period - elapsed);
    elapsed = glfw::time() - m_lastUpdate;
  }
}

