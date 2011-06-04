#include "Renderer.h"
#include <boost/foreach.hpp>
#include <GL/glfw.h>
#include <GL/glu.h>
#include <cmath>
#include "RendererContext.h"
#include "DataStore.h"

namespace
{
  double rad2deg(double rad)
  {
    return 57.29577951308232087721 * rad;
  }

  unsigned int createGridDisplayList()
  {
    static const unsigned int NUM_X_LINES = 30;
    static const unsigned int NUM_Y_LINES = 30;
    static const double dx = 20.0;
    static const double dy = 20.0;

    unsigned int index = glGenLists(1);

    glNewList(index, GL_COMPILE);
   
    glDisable(GL_TEXTURE_2D);

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
    
    glEnable(GL_TEXTURE_2D);
    
    glEndList();

    return index;
  }

  unsigned int createMoonDisplayList(const TextureManager& textures)
  {
    unsigned int index = glGenLists(1);

    glNewList(index, GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, textures.getTextureHandle("moon"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0, -1.0);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0, 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0, 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0, -1.0);
    glEnd();

    glEndList();

    return index;
  }

  unsigned int createAvatarDisplayList(const TextureManager& textures)
  {
    unsigned int index = glGenLists(1);

    glNewList(index, GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, textures.getTextureHandle("avatar"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.25, -0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.25, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.25, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.25, -0.5);
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
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

  glViewport(0,0,600,600);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-300,300,-300,300);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  const std::string path(DataStore::get<std::string>("ResourcePath"));
  
  m_textures.loadTexture(path + DataStore::get<std::string>("MoonImg"), "moon");
  m_textures.loadTexture(path + DataStore::get<std::string>("Avatar"), "avatar");

  m_moonDisplayList = createMoonDisplayList(m_textures);
  m_avatarDisplayList = createAvatarDisplayList(m_textures);
  m_gridDisplayList = createGridDisplayList();
}

void Renderer::render(const RendererContext& context) const
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  renderGrid(context);
  renderMoons(context);
  renderAvatar(context);
  
  glfwSwapBuffers();
}

void Renderer::renderMoons(const RendererContext& context) const
{
  BOOST_FOREACH(const Moon& moon, context.getMoons())
  {
    glPushMatrix();
    glTranslatef(moon.x, moon.y, 0.0);
    glRotatef(rad2deg(moon.theta), 0.0, 0.0, 1.0);
    glScalef(moon.r,moon.r,moon.r);
    glCallList(m_moonDisplayList);
    glPopMatrix();
  }
}

void Renderer::renderGrid(const RendererContext& context) const
{
  glCallList(m_gridDisplayList);
}

void Renderer::renderAvatar(const RendererContext& context) const
{
  const Moon& moon = context.getMoons().back();
  const Avatar& avatar = context.getAvatar();

  glPushMatrix();
  glTranslatef(moon.x, moon.y, 0.0);
  glRotatef(rad2deg(moon.theta + avatar.theta), 0.0, 0.0, 1.0);
  glTranslatef(0.0, 0.5 * avatar.height + moon.r, 0.0);
  glScalef(avatar.height, avatar.height, avatar.height);
  glCallList(m_avatarDisplayList);
  glPopMatrix();
}
