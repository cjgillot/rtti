#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

file(GLOB src *.cpp)

foreach(t IN ITEMS ${src})
  get_filename_component(n ${t} NAME_WE)
  add_executable(test-smart_ptr-${n}.exe ${t} ../test.cpp ../classes.cpp)
  target_link_libraries(test-smart_ptr-${n}.exe rtti ${Boost_LIBRARIES})
  add_test(smart_ptr-${n}.test test-smart_ptr-${n}.exe)
endforeach(t)
