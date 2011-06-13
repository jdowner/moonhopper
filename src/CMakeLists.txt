set(CMAKE_CXX_FLAGS -DTIXML_USE_STL)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
set(SRC
  AABB.cpp
  AABB.h
  Animation.cpp
  Animation.h
  Animation_TestSuite.cpp
  Animation_TestSuite.h
  AnimationFrame.h
  ArgumentHandler.cpp
  ArgumentHandler.h
  CollisionDetection.cpp
  CollisionDetection.h
  CollisionResolution.cpp
  CollisionResolution.h
  DataStore.cpp
  DataStore.h
  Engine.cpp
  Engine.h
  main.cpp
  MathUtils.cpp
  MathUtils.h
  Moon.cpp
  Moon.h
  PeriodicDomain.cpp
  PeriodicDomain.h
  PeriodicDomain_TestSuite.cpp
  PeriodicDomain_TestSuite.h
  Renderer.cpp
  Renderer.h
  TestDriver.cpp
  TestDriver.h
  TestUtils.cpp
  TestUtils.h
  TextureManager.cpp
  TextureManager.h
  UpdateContext.h
  Universe.cpp
  Universe.h
  Vector2d.cpp
  Vector2d.h
)

include_directories(../tinyxml)
link_directories(../lib)

add_executable(moonhopper ${SRC})

target_link_libraries(moonhopper GL GLU glfw png tinyxml)
