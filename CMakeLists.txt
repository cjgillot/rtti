#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 2.8)
project(rtti)

set(CMAKE_BUILD_TYPE Release)

include_directories(include)
add_definitions(-std=c++11 -g -O0)
# add_definitions(-std=c++11 -O2 -pedantic-errors -Wall -Wextra)

add_subdirectory(examples)
add_subdirectory(include)
add_subdirectory(src)
