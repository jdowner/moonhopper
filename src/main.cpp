
#include <stdexcept>
#include <iostream>
#include <GL/glfw.h>
#include <GL/glu.h>
#include <cstdlib>

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


int main(int argc, char** argv)
{
  try
  {
    int running = GL_TRUE;

    if (!glfwInit())
    {
      exit(EXIT_FAILURE);
    }

    if (!glfwOpenWindow(300,300,0,0,0,0,0,0,GLFW_WINDOW))
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
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

    while (running)
    {
      glClear(GL_COLOR_BUFFER_BIT);
      glLoadIdentity();
      drawQuad();
      glfwSwapBuffers();
      running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
      glfwSleep(0.01);
      angle += 1;
    }

    glfwTerminate();

    exit(EXIT_SUCCESS);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Encountered unrecognized exception" << std::endl;
  }

  return 0;
}
