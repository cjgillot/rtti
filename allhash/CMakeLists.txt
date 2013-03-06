add_subdirectory(mph)
add_subdirectory(visitor)
add_subdirectory(multimethod)

file(GLOB_RECURSE src *.cpp)
file(GLOB_RECURSE hdr *.hpp *.ipp)

add_library(rtti_allhash ${src} ${hdr})
