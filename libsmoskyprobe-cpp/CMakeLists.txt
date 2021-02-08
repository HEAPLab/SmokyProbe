cmake_minimum_required(VERSION 3.5.1)

project(smokyprobe)

############################################################
# Create a library
############################################################


set (CMAKE_CXX_FLAGS "-std=c++11")

#Generate the shared library from the library sources
add_library(smokyprobe SHARED 
    src/measurable.cpp
)

set_target_properties(smokyprobe PROPERTIES SOVERSION 1)

target_include_directories(smokyprobe PUBLIC ${PROJECT_SOURCE_DIR}/include)
