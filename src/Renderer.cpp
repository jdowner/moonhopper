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

    const unsigned int NUM_X_LINES = 
      DataStore::get<unsigned int>("NumGridLinesX", 31);
    const unsigned int NUM_Y_LINES = 
      DataStore::get<unsigned int>("NumGridLinesY", 31);

    const double minX = DataStore::get<double>("DomainMinX", -300.0);
    const double minY = DataStore::get<double>("DomainMinY", -300.0);
    const double maxX = DataStore::get<double>("DomainMaxX", 300.0);
    const double maxY = DataStore::get<double>("DomainMaxY", 300.0);
    
    const double dx = (maxX - minX) / (NUM_X_LINES + 1);
    const double dy = (maxY - minY) / (NUM_Y_LINES + 1);
    
    for (unsigned int i = 0; i < NUM_X_LINES; ++i)
    {
      glBegin(GL_LINES);
      glVertex2f(i * dx + minX, minY);
      glVertex2f(i * dx + minX, maxY);
      glEnd();
    }
    for (unsigned int i = 0; i < NUM_Y_LINES; ++i)
    {
      glBegin(GL_LINES);
      glVertex2f(minX, i * dy + minY);
      glVertex2f(maxX, i * dy + minY);
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
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.25, 0.0);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.25, 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.25, 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.25, 0.0);
    glEnd();

    glEndList();

    return index;
  }

  class RenderMoonOp : public MoonConstOperation
  {
    public:
      RenderMoonOp(unsigned int displayList)
      : m_displayList(displayList)
      {
      }

      virtual void execute(const Moon& moon)
      {
        glPushMatrix();
        glTranslatef(moon.x, moon.y, 0.0);
        glRotatef(rad2deg(moon.theta), 0.0, 0.0, 1.0);
        glScalef(moon.r,moon.r,moon.r);
        glCallList(m_displayList);
        glPopMatrix();
      }

    private:
      unsigned int m_displayList;
  };

  class RenderAvatarOp : public AvatarConstOperation
  {
    public:
      RenderAvatarOp(const RendererContext& context, unsigned int displayList)
      : m_context(context)
      , m_displayList(displayList)
      {
      }

      virtual void execute(const Avatar& avatar)
      {
        const Moon* moon = avatar.moon;

        assert(moon);

        glPushMatrix();
        glTranslatef(moon->x, moon->y, 0.0);
        glRotatef(rad2deg(moon->theta + avatar.theta), 0.0, 0.0, 1.0);
        glTranslatef(0.0, moon->r, 0.0);
        glScalef(avatar.height, avatar.height, avatar.height);
        glCallList(m_displayList);
        glPopMatrix();

        // if avatar is jumping, draw ray
        if (m_context.isJumping())
        { 
          const Ray& ray = m_context.getRay();

          glDisable(GL_TEXTURE_2D);
          glColor3ub(255,0,255);
          glBegin(GL_LINES);
          glVertex2f(ray.ox, ray.oy);
          glVertex2f(ray.ox + 100.0 * ray.nx, ray.oy + 100.0 * ray.ny);
          glEnd();
          glEnable(GL_TEXTURE_2D);
        }
      }

    private:
      unsigned int m_displayList;
      const RendererContext& m_context;
  };
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

  const double minX = DataStore::get<double>("DomainMinX", -300.0);
  const double minY = DataStore::get<double>("DomainMinY", -300.0);
  const double maxX = DataStore::get<double>("DomainMaxX", 300.0);
  const double maxY = DataStore::get<double>("DomainMaxY", 300.0);

  glViewport(0, 0, maxX - minX, maxY - minY);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(minX, maxX, minY, maxY);
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
  RenderMoonOp op(m_moonDisplayList);
  context.execute(op);
}

void Renderer::renderGrid(const RendererContext& context) const
{
  glCallList(m_gridDisplayList);
}

void Renderer::renderAvatar(const RendererContext& context) const
{
  RenderAvatarOp op(context, m_avatarDisplayList);
  context.execute(op);
}
