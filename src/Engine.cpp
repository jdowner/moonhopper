#include "Engine.h"
#include <stdexcept>
#include <GL/glfw.h>
#include <GL/glu.h>

Engine::Engine() : m_running(false)
{
}

void Engine::init()
{
  if (!glfwInit())
  {
    throw std::runtime_error("Unable to initialize glfw");
  }

  if (!glfwOpenWindow(300,300,0,0,0,0,0,0,GLFW_WINDOW))
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
}

