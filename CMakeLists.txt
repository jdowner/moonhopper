cmake_minimum_required(VERSION 2.6)

project(MoonHopper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

find_package(glfw REQUIRED)

add_subdirectory(src)
