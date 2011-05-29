#include "Renderer.h"
#include <boost/foreach.hpp>
#include <GL/glfw.h>
#include <GL/glu.h>
#include "RendererContext.h"

void Renderer::init()
{
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
}

void Renderer::render(const RendererContext& context) const
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  
  renderGrid(context);
  renderMoons(context);
  
  glfwSwapBuffers();
}

void Renderer::renderMoons(const RendererContext& context) const
{
  BOOST_FOREACH(const Moon& moon, context.getMoons())
  {
    glColor3ub(0x8C, 0xAE, 0x3C);

    glBegin(GL_QUADS);
    glVertex2f(moon.x - moon.r, moon.y - moon.r);
    glVertex2f(moon.x + moon.r, moon.y - moon.r);
    glVertex2f(moon.x + moon.r, moon.y + moon.r);
    glVertex2f(moon.x - moon.r, moon.y + moon.r);
    glEnd();
  }
}

void Renderer::renderGrid(const RendererContext& context) const
{
  const unsigned int NUM_X_LINES = 30;
  const unsigned int NUM_Y_LINES = 30;
  const double dx = 20.0;
  const double dy = 20.0;
  glColor3ub(0x22,0x22,0x22);

  glBegin(GL_POLYGON);
  glVertex2f(-10.0,-10.0);
  glVertex2f(+10.0,-10.0);
  glVertex2f(+10.0,+10.0);
  glVertex2f(-10.0,+10.0);
  glEnd();
  
  for (unsigned int i = 0; i < NUM_X_LINES; ++i)
  {
    glBegin(GL_LINES);
    glVertex2f(i * dx - 300.0, -300.0);
    glVertex2f(i * dx - 300.0, 300.0);
    glEnd();
  }
  for (unsigned int i = 0; i < NUM_Y_LINES; ++i)
  {
    glBegin(GL_LINES);
    glVertex2f(-300.0, i * dy - 300.0);
    glVertex2f(300.0, i * dy - 300.0);
    glEnd();
  }
}
