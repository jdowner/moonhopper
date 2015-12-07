set(CMAKE_CXX_FLAGS -DTIXML_USE_STL)
set(EXECUTABLE_OUTPUT_PATH ${MOONHOPPER_BIN_PATH})

include_directories(../tinyxml /usr/include/freetype2)
link_directories(${MOONHOPPER_LIB_PATH}/lib)

add_definitions(-std=c++11)
add_executable(moonhopper
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
  GLFW.cpp
  GLFW.h
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
  TetheredMoons.cpp
  TetheredMoons.h
  TextureManager.cpp
  TextureManager.h
  UpdateContext.h
  Universe.cpp
  Universe.h
  Vector2d.cpp
  Vector2d.h
  )

target_link_libraries(moonhopper GL GLU glfw png tinyxml ftgl)
