add_subdirectory(mmethod)

file(GLOB_RECURSE src *.cpp)

add_definitions(-O3 -fexpensive-optimizations -fomit-frame-pointer -funroll-all-loops -DNDEBUG)
add_definitions(-fno-rtti -fexceptions -funroll-loops -pedantic -Wabi -march=corei7 -mtune=corei7)
add_definitions(-Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn)
add_library(rtti ${src})
