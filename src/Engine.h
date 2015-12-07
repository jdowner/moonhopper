#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include "Renderer.h"
#include "Universe.h"
#include "PeriodicDomain.h"
#include "CollisionResolution.h"
#include "UpdateContext.h"
#include "GLFW.h"

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
    void sleep();

  private:
    UpdateContext createUpdateContext() const;

  private:
    Universe m_universe;
    Renderer m_renderer;
    glfw::Window* m_window;
    bool m_running;

    /**
     * Defines the frequency (Hz) at which the updates are called.
     */
    double m_frameRate;

    /**
     * The last time that the update function was called (seconds).
     */
    double m_lastUpdate;
};

#endif // ENGINE_H

