add_subdirectory(mmethod)

file(GLOB_RECURSE src *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/cache-table-0.2)

# add_definitions(-O3 -fexpensive-optimizations -DNDEBUG -fomit-frame-pointer)
# add_definitions(-fno-rtti -fexceptions -funroll-loops -pedantic -march=corei7 -mtune=corei7)
# add_definitions(-Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn)
add_library(rtti ${src})
