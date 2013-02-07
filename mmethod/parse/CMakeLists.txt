file(GLOB src *.cpp)

add_executable(parse ${src})
target_link_libraries(parse boost_serialization boost_regex boost_program_options)
