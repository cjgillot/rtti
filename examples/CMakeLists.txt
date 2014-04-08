#          Copyright Camille Gillot 2012 - 2014.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

# RTTI
add_subdirectory(basic)
add_subdirectory(multiple)
add_subdirectory(hierarchy)

# MMETHOD
add_subdirectory(unary)
add_subdirectory(binary)
add_subdirectory(diamond)
add_subdirectory(prefetch)
add_subdirectory(multifile)
add_subdirectory(smart_ptr)
add_subdirectory(predator_prey)

# TESTSUITE
file(GLOB_RECURSE tests *.cpp)

add_executable(testsuite ${tests})
target_link_libraries(testsuite rtti)
target_link_libraries(testsuite -static boost_unit_test_framework)
