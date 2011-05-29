
set(SRC
  Engine.cpp
  Engine.h
  main.cpp
  RendererContext.cpp
  RendererContext.h
  Renderer.cpp
  Renderer.h
)

add_executable(moonhopper ${SRC})

target_link_libraries(moonhopper GL GLU glfw)

