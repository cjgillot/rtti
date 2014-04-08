#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

add_subdirectory(dispatch)

file(GLOB_RECURSE src *.cpp)

include_directories(.)

add_definitions(-O3 -fexpensive-optimizations -DNDEBUG -fomit-frame-pointer)
add_definitions(-funroll-loops -pedantic -march=corei7 -mtune=corei7)
add_library(rtti SHARED ${src})
