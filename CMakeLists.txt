cmake_minimum_required(VERSION 2.6)

project(MoonHopper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set(MOONHOPPER_LIB_PATH ${CMAKE_BINARY_DIR}/lib)
set(MOONHOPPER_BIN_PATH ${CMAKE_BINARY_DIR}/bin)
set(MOONHOPPER_RES_PATH ${CMAKE_BINARY_DIR}/share/moonhopper)

option(USE_GLFW3 "Build moonhopper using the GLFW3 library" OFF)

if(USE_GLFW3)
  add_definitions(-DUSE_GLFW3)
  find_package(GLFW3)
else()
  find_package(GLFW)
endif()

find_package(FTGL)

add_subdirectory(src)
add_subdirectory(tinyxml)
add_subdirectory(resources)
