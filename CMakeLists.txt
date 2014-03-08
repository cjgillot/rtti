cmake_minimum_required(VERSION 2.8)
project(rtti)

set(CMAKE_BUILD_TYPE Release)

include_directories(include)
add_definitions(-std=c++11 -g -O0)
# add_definitions(-std=c++11 -O2 -pedantic-errors -Wall -Wextra)

add_subdirectory(examples)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(doc)
