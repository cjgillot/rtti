#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

file(GLOB src *.cpp)

# for foreach.hpp
include_directories(${CMAKE_SOURCE_DIR}/src)

foreach(t IN ITEMS ${src})
  get_filename_component(n ${t} NAME_WE)
  add_executable(test-policy-${n}.exe ${t} ../test.cpp ../classes.cpp)
  target_link_libraries(test-policy-${n}.exe rtti ${Boost_LIBRARIES})
  add_test(policy-${n}.test test-policy-${n}.exe)
endforeach(t)
