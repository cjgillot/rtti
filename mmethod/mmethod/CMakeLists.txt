set(USE_JENKINS OFF)

if(${USE_JENKINS})

file(GLOB gen_mph_src
  ${CMAKE_SOURCE_DIR}/gen_mph/lookupa.c
  ${CMAKE_SOURCE_DIR}/gen_mph/perfect.c
  ${CMAKE_SOURCE_DIR}/gen_mph/perfhex.c
  ${CMAKE_SOURCE_DIR}/gen_mph/recycle.c
  gen_mph.cc
)

add_library(gen_mph ${gen_mph_src})
set_property(TARGET gen_mph APPEND PROPERTY COMPILE_DEFINITIONS GEN_MPH_CPP=1)

else(${USE_JENKINS})

add_library(gen_mph gperf.cc)
set_property(TARGET gen_mph APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/mph/)
set_property(TARGET gen_mph APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/gperf-3.0.4/src)
set_property(TARGET gen_mph APPEND PROPERTY COMPILE_DEFINITIONS GPERF_CPP=1)
set_property(TARGET gen_mph APPEND PROPERTY COMPILE_FLAGS "-Werror -Wno-shadow")
link_directories(${CMAKE_SOURCE_DIR}/gperf-3.0.4/lib)
link_directories(${CMAKE_SOURCE_DIR}/gperf-3.0.4/src)

endif(${USE_JENKINS})

file(GLOB mph_src *.cpp *.hpp)

add_executable(mmethod ${mph_src})
target_link_libraries(mmethod gen_mph gperf gp boost_serialization boost_program_options)
utility_source(MMETHOD mmethod . ${mph_src})
