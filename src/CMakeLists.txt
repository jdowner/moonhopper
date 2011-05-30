
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
set(SRC
  ArgumentHandler.cpp
  ArgumentHandler.h
  DataStore.cpp
  DataStore.h
  Engine.cpp
  Engine.h
  main.cpp
  Moon.cpp
  Moon.h
  RendererContext.cpp
  RendererContext.h
  Renderer.cpp
  Renderer.h
  TestDriver.cpp
  TestDriver.h
)

add_executable(moonhopper ${SRC})

target_link_libraries(moonhopper GL GLU glfw)
