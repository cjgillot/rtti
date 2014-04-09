#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

file(GLOB_RECURSE source *.cpp)

add_definitions(-DNDEBUG -fno-exceptions -fno-rtti -march=corei7 -mtune=corei7 -pedantic -fomit-frame-pointer -O3 -fexpensive-optimizations)

add_executable(benchmark ${source})
target_link_libraries(benchmark rtti)
