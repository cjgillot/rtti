set(USE_JENKINS OFF)

if(${USE_JENKINS})

file(GLOB gen_mph_src
  gen_mph/lookupa.c gen_mph/perfect.c gen_mph/perfhex.c gen_mph/recycle.c gen_mph.cpp
)

add_library(gen_mph ${gen_mph_src})
set_property(TARGET gen_mph APPEND PROPERTY COMPILE_DEFINITIONS GEN_MPH_CPP=1)

else(${USE_JENKINS})

file(GLOB gperf_src gperf.cc)

add_library(gen_mph_allhash ${gperf_src})
set_property(TARGET gen_mph_allhash        PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/rtti/mph/)
set_property(TARGET gen_mph_allhash APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/rtti/mph/gperf-3.0.4/src)
set_property(TARGET gen_mph_allhash APPEND PROPERTY COMPILE_DEFINITIONS GPERF_CPP=1)
set_property(TARGET gen_mph_allhash APPEND PROPERTY COMPILE_FLAGS "-Werror -Wno-shadow")
link_directories(${CMAKE_SOURCE_DIR}/src/rtti_allhash/mph/gperf-3.0.4/lib)
link_directories(${CMAKE_SOURCE_DIR}/src/rtti_allhash/mph/gperf-3.0.4/src)

endif(${USE_JENKINS})

file(GLOB mph_src *.cpp *.hpp)

add_executable(mph_allhash ${mph_src})
target_link_libraries(mph_allhash gen_mph gperf gp boost_serialization)
utility_source(MPH mph . ${mph_src})


add_executable(parse_allhash parse.cc)
target_link_libraries(parse_allhash boost_regex boost_serialization)

# add_executable(syntax syntax.cc)
# target_link_libraries(parse boost_regex boost_serialization)
