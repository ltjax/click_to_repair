cmake_minimum_required(VERSION 3.13)
project(click_to_repair)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS KEEP_RPATHS)

find_package(SDL2 REQUIRED)
find_package(glew REQUIRED)

add_subdirectory(externals)
add_subdirectory(src)
