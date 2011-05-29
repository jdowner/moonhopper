#include "Engine.h"
#include <stdexcept>
#include <GL/glfw.h>
#include <GL/glu.h>

float angle = 0.0;

void drawQuad()
{
  glColor3ub(0x8C, 0xAE, 0x3C);
  glRotatef(angle, 0, 0, 1);
  
  glBegin(GL_QUADS);
  glVertex2f(0.0f,0.0f);
  glVertex2f(100.0f,0.0f);
  glVertex2f(100.0f,100.0f);
  glVertex2f(0.0f,100.0f);
  glEnd();
}



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

  glfwSwapInterval(0);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glDisable( GL_DEPTH_TEST );
  glEnable( GL_TEXTURE_2D );
  glEnable( GL_BLEND );

  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0,0,300,300);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-300,300,-300,300);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

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
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  drawQuad();
  glfwSwapBuffers();
  m_running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
  glfwSleep(0.01);
  angle += 1;
}

void Engine::update()
{
}

