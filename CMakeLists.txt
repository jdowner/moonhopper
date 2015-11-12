cmake_minimum_required(VERSION 2.6)

project(MoonHopper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(FTGL)
find_package(GLFW)

set(MOONHOPPER_LIB_PATH ${CMAKE_BINARY_DIR}/lib)
set(MOONHOPPER_BIN_PATH ${CMAKE_BINARY_DIR}/bin)
set(MOONHOPPER_RES_PATH ${CMAKE_BINARY_DIR}/share)

add_subdirectory(src)
add_subdirectory(tinyxml)
add_subdirectory(resources)
