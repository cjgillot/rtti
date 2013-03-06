add_subdirectory(mmethod)

file(GLOB_RECURSE src *.cpp)

add_definitions(-O3 -fexpensive-optimizations -fomit-frame-pointer -funroll-loops -flto -DNDEBUG)
add_definitions(-fno-rtti -fno-exceptions -funroll-loops)
add_library(rtti ${src})
