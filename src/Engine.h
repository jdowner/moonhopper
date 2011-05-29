#ifndef ENGINE_H
#define ENGINE_H

#include "Renderer.h"

/**
 *
 */
class Engine
{ 
  public:
    Engine();

    void init();
    void shutdown();
    bool running() const;

    void render();
    void update();

  private:
    Renderer m_renderer;
    bool m_running;
};

#endif // ENGINE_H

