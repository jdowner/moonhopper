#include "Renderer.h"
#include <sstream>
#include <boost/foreach.hpp>
#include <GL/glu.h>
#include <FTGL/ftgl.h>
#include <cmath>
#include "GLFW.h"
#include "Universe.h"
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

  unsigned int createHookDisplayList(const TextureManager& textures)
  {
    unsigned int index = glGenLists(1);

    glNewList(index, GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, textures.getTextureHandle("hook"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5, 0.0);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5, 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5, 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5, 0.0);
    glEnd();

    glEndList();

    return index;
  }
}

void Renderer::init()
{
  glfw::swapInterval(0);

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
  m_textures.loadTexture(path + DataStore::get<std::string>("Hook"), "hook");

  m_moonDisplayList = createMoonDisplayList(m_textures);
  m_avatarDisplayList = createAvatarDisplayList(m_textures);
  m_hookDisplayList = createHookDisplayList(m_textures);
  m_gridDisplayList = createGridDisplayList();
}

void Renderer::render(const Universe& universe) const
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  renderGrid(universe);
  renderMoons(universe);

  if (!universe.isAvatarDead())
  {
    renderAvatar(universe);
    renderHook(universe);
  }
  else
  {
    renderDeathScreen(universe);
  }
}

void Renderer::renderMoons(const Universe& universe) const
{
  BOOST_FOREACH(Moon* moon, universe.getMoons())
  {
    glPushMatrix();
    glTranslatef(moon->x, moon->y, 0.0);
    glRotatef(rad2deg(moon->theta), 0.0, 0.0, 1.0);
    glScalef(moon->r,moon->r,moon->r);
    glCallList(m_moonDisplayList);
    glPopMatrix();

    if (DataStore::get<bool>("Debug", false) && universe.isTethered(moon))
    {
      const double x = moon->x;
      const double y = moon->y + moon->r + 5;
      const double minx = DataStore::get<double>("DomainMinX");
      const double miny = DataStore::get<double>("DomainMinY");

      renderText(x - minx,y - miny,"tethered");

      std::stringstream u;
      u << moon->u;
      renderText(x - minx,y - miny + 10,u.str().c_str());

      std::stringstream v;
      v << moon->v;
      renderText(x - minx,y - miny + 20,v.str().c_str());
    }
  }
}

void Renderer::renderGrid(const Universe& universe) const
{
  glCallList(m_gridDisplayList);
}

void Renderer::renderAvatar(const Universe& universe) const
{
  const Avatar& avatar = universe.getAvatar();
  const Moon* moon = avatar.moon;

  assert(moon);

  glPushMatrix();
  glTranslatef(moon->x, moon->y, 0.0);
  glRotatef(rad2deg(moon->theta + avatar.theta), 0.0, 0.0, 1.0);
  glTranslatef(0.0, moon->r, 0.0);
  glScalef(avatar.height, avatar.height, avatar.height);
  glCallList(m_avatarDisplayList);
  glPopMatrix();

  if (DataStore::get<bool>("Debug", false))
  {
    // The debugging text should appear a little bit above the avatars head
    const double theta = moon->theta + avatar.theta + M_PI/2.0;
    const double radius = moon->r + 1.5 * avatar.height;
    const double x = moon->x + radius * cos(theta);
    const double y = moon->y + radius * sin(theta);
    const double minx = DataStore::get<double>("DomainMinX");
    const double miny = DataStore::get<double>("DomainMinY");

    renderText(x - minx,y - miny,"avatar");
  }

  // if avatar is jumping, draw ray
  if (avatar.isJumping)
  {
    const Ray& ray = avatar.up;

    glDisable(GL_TEXTURE_2D);
    glColor3ub(255,0,255);
    glBegin(GL_LINES);
    glVertex2f(ray.ox, ray.oy);
    glVertex2f(ray.ox + 100.0 * ray.nx, ray.oy + 100.0 * ray.ny);
    glEnd();
    glEnable(GL_TEXTURE_2D);
  }
}

void Renderer::renderHook(const Universe& universe) const
{
  if (universe.isHookExtant())
  {
    const Hook& hook = universe.getHook();

    glPushMatrix();
    glTranslatef(hook.position.x, hook.position.y, 0.0);
    glRotatef(rad2deg(hook.angle), 0.0, 0.0, 1.0);
    glScalef(hook.radius, hook.radius, hook.radius);
    glCallList(m_hookDisplayList);
    glPopMatrix();
  }
}

void Renderer::renderDeathScreen(const Universe& universe) const
{
}

void Renderer::renderText(int x, int y, const std::string& text) const
{
  FTPixmapFont font(DataStore::get<std::string>("DebugFontFamily").c_str());

  font.FaceSize(DataStore::get<int>("DebugFontSize"));
  font.Render(text.c_str(),-1,FTPoint(x,y));
}

