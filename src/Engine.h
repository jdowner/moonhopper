#ifndef ENGINE_H
#define ENGINE_H

#include "Renderer.h"
#include "RendererContext.h"
#include "PeriodicDomain.h"

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
    void updateMoonPositions();
    void resolveCollisions();

  private:
    RendererContext m_context;
    Renderer m_renderer;
    bool m_running;

    /**
     * Defines a periodic domain that the moons move in
     */
    PeriodicDomain m_domain;

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

