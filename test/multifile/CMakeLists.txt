#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

file(GLOB src *.cpp)

add_executable(test-multifile.exe ${src} ../test.cpp)
target_link_libraries(test-multifile.exe rtti ${Boost_LIBRARIES})
add_test(multifile.test test-multifile.exe)
