#include "Renderer.h"
#include <boost/foreach.hpp>
#include <GL/glfw.h>
#include <GL/glu.h>
#include <cmath>
#include "RendererContext.h"

namespace
{
  unsigned int createGridDisplayList()
  {
    static const unsigned int NUM_X_LINES = 30;
    static const unsigned int NUM_Y_LINES = 30;
    static const double dx = 20.0;
    static const double dy = 20.0;

    unsigned int index = glGenLists(1);

    glNewList(index, GL_COMPILE);
    
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
    
    glEndList();

    return index;
  }

  unsigned int createMoonDisplayList()
  {
    static const unsigned int NUM_VERTICES = 32;
    static const double RADIUS = 10.0;
    static const double dTHETA = 2.0 * M_PI / NUM_VERTICES;

    unsigned int index = glGenLists(1);

    glNewList(index, GL_COMPILE);
    
    glColor3ub(0x8C, 0xAE, 0x3C);

    glBegin(GL_POLYGON);
    for (unsigned int i = 0; i < NUM_VERTICES; ++i)
    {
      glVertex2f(RADIUS * cos(i * dTHETA), RADIUS * sin(i * dTHETA));
    }
    glEnd();

    glEndList();

    return index;
  }
}

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

  m_moonDisplayList = createMoonDisplayList();
  m_gridDisplayList = createGridDisplayList();
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
    glPushMatrix();
    glTranslatef(moon.x, moon.y, 0.0);
    glCallList(m_moonDisplayList);
    glPopMatrix();
  }
}

void Renderer::renderGrid(const RendererContext& context) const
{
  glCallList(m_gridDisplayList);
}
