#ifndef RENDERER_H
#define RENDERER_H

#include "TextureManager.h"

class Universe;

/**
 *
 */
class Renderer
{
  public:
    void init();
    void render(const Universe& universe) const;

  private:
    void renderMoons(const Universe& universe) const;
    void renderGrid(const Universe& universe) const;
    void renderAvatar(const Universe& universe) const;

  private:
    unsigned int m_moonDisplayList;
    unsigned int m_gridDisplayList;
    unsigned int m_avatarDisplayList;
    TextureManager m_textures;
};

#endif // RENDERER_H

