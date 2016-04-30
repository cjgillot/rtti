#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 2.8)
project(rtti)

set(CMAKE_BUILD_TYPE Release)

find_package(Boost
  1.55
  REQUIRED
  COMPONENTS
    unit_test_framework
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

add_definitions(-pedantic-errors -Wall -Wextra)
add_definitions(-std=c++11)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)
