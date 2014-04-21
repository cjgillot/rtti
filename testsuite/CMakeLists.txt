#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

add_subdirectory(rtti)
add_subdirectory(mmethod)
add_subdirectory(multifile)

add_subdirectory(policy)
add_subdirectory(smart_ptr)

# TESTSUITE
file(GLOB_RECURSE tests *.cpp)

add_executable(testsuite ${tests})
target_link_libraries(testsuite rtti boost_unit_test_framework boost_exception)
