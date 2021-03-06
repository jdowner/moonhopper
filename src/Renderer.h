#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "TextureManager.h"

class Universe;

namespace glfw
{
  class Window;
}

/**
 *
 */
class Renderer
{
  public:
    void init();
    void render(const Universe& universe) const;
    void renderText(int x, int y, const std::string& text) const;

    void createWindow(int width, int height, const char* title);

  private:
    void renderMoons(const Universe& universe) const;
    void renderGrid(const Universe& universe) const;
    void renderAvatar(const Universe& universe) const;
    void renderHook(const Universe& universe) const;
    void renderDeathScreen(const Universe& universe) const;

  private:
    unsigned int m_moonDisplayList;
    unsigned int m_gridDisplayList;
    unsigned int m_avatarDisplayList;
    unsigned int m_hookDisplayList;
    TextureManager m_textures;
};

#endif // RENDERER_H

