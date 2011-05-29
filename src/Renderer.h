#ifndef RENDERER_H
#define RENDERER_H

class RendererContext;

/**
 *
 */
class Renderer
{
  public:
    void init();
    void render(const RendererContext& context) const;

  private:
    void renderMoons(const RendererContext& context) const;
    void renderGrid(const RendererContext& context) const;
};

#endif // RENDERER_H

