#ifndef GLFW_H
#define GLFW_H

#ifdef USE_GLFW3
class GLFWwindow;
#endif // USE_GLFW3

namespace glfw
{
  void init();
  void terminate();

  void sleep(double duration);
  double time();
  int getKey(int key);
  void swapInterval(int interval);

  /**
   * This class is used to abstract the GLFW interface in order to support both
   * GLFW and GLFW3.
   */
  class Window
  {
    public:
      Window(int width, int height, const char* title);
      explicit Window(const Window& window);

    public:
      void swapBuffers() const;
      void makeCurrent() const;
      void pollEvents() const;

      bool isOpen() const;
      int getKey(int key) const;

    public:
#ifdef USE_GLFW3
      mutable GLFWwindow* m_window;
#endif // USE_GLFW3
  };

  extern const int KEY_LEFT;
  extern const int KEY_RIGHT;
  extern const int KEY_UP;
  extern const int KEY_DOWN;
  extern const int KEY_TAB;
  extern const int KEY_ESC;

  extern const int KEY_G;
  extern const int KEY_H;

  extern const int KEY_PRESS;
  extern const int KEY_RELEASE;
}

#endif // GLFW_H
