cmake_minimum_required(VERSION 2.6)

project(MoonHopper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

option(USE_GLFW3 "Build moonhopper using the GLFW3 library" OFF)

if(USE_GLFW3)
  add_definitions(-DUSE_GLFW3)
endif()

find_package(GLFW)
find_package(FTGL)

message(STATUS "glfw library: ${GLFW_LIBRARIES}")

set(MOONHOPPER_LIB_PATH ${CMAKE_BINARY_DIR}/lib)
set(MOONHOPPER_BIN_PATH ${CMAKE_BINARY_DIR}/bin)
set(MOONHOPPER_RES_PATH ${CMAKE_BINARY_DIR}/share)

add_subdirectory(src)
add_subdirectory(tinyxml)
add_subdirectory(resources)
