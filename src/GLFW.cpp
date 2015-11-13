#include "GLFW.h"
#include <stdexcept>

#ifdef USE_GLFW3
  #include <GLFW/glfw3.h>
#else
  #include <GL/glfw.h>
#endif // USE_GLFW3


namespace glfw
{
#ifdef USE_GLFW3
  const int KEY_LEFT          = GLFW_KEY_LEFT;
  const int KEY_RIGHT         = GLFW_KEY_RIGHT;
  const int KEY_UP            = GLFW_KEY_UP;
  const int KEY_DOWN          = GLFW_KEY_DOWN;
  const int KEY_TAB           = GLFW_KEY_TAB;
  const int KEY_ESC           = GLFW_KEY_ESCAPE;

  const int KEY_G             = GLFW_KEY_G;
  const int KEY_H             = GLFW_KEY_H;

  const int KEY_PRESS         = GLFW_PRESS;
  const int KEY_RELEASE       = GLFW_RELEASE;

#else
  const int KEY_LEFT          = GLFW_KEY_LEFT;
  const int KEY_RIGHT         = GLFW_KEY_RIGHT;
  const int KEY_UP            = GLFW_KEY_UP;
  const int KEY_DOWN          = GLFW_KEY_DOWN;
  const int KEY_TAB           = GLFW_KEY_TAB;
  const int KEY_ESC           = GLFW_KEY_ESC;

  const int KEY_G             = 71;
  const int KEY_H             = 72;

  const int KEY_PRESS         = GLFW_PRESS;
  const int KEY_RELEASE       = GLFW_RELEASE;

#endif // USE_GLFW3

  void init()
  {
    if (!glfwInit())
    {
      throw std::runtime_error("Unable to initialize glfw");
    }
  }

  void terminate()
  {
    glfwTerminate();
  }

  double time()
  {
    return glfwGetTime();
  }

  void sleep(double duration)
  {
#ifdef USE_GLFW3
#else
    glfwSleep(duration);
#endif // USE_GLFW3
  }

  void swapInterval(int interval)
  {
    glfwSwapInterval(interval);
  }

  Window::Window(int width, int height, const char* title)
  {
#ifdef USE_GLFW3
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
#else
    if (!glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
      throw std::runtime_error("Unable to create window");
    }
#endif // USE_GLFW3
  }

  Window::Window(const Window& window)
  {
#ifdef USE_GLFW3
    m_window = window.m_window;
#endif // USE_GLFW3
  }

  void Window::swapBuffers() const
  {
#ifdef USE_GLFW3
    glfwSwapBuffers(m_window);
#else
    glfwSwapBuffers();
#endif // USE_GLFW3
  }

  bool Window::isOpen() const
  {
#ifdef USE_GLFW3
    return true;
#else
    return glfwGetWindowParam(GLFW_OPENED);
#endif // USE_GLFW3
  }

  void Window::makeCurrent() const
  {
#ifdef USE_GLFW3
    glfwMakeContextCurrent(m_window);
#endif // USE_GLFW3
  }

  void Window::pollEvents() const
  {
#ifdef USE_GLFW3
    glfwPollEvents();
#endif // USE_GLFW3
  }

  int Window::getKey(int key) const
  {
#ifdef USE_GLFW3
    return glfwGetKey(m_window, key);
#else
    return glfwGetKey(key);
#endif // USE_GLFW3
  }
}
