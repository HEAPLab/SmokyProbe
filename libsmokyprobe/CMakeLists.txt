cmake_minimum_required(VERSION 3.9)

project(smokyprobe VERSION 1.0 DESCRIPTION "SmokyProbe PC-side library")

############################################################
# Create a library
############################################################

#Generate the shared library from the library sources
add_library(smokyprobe SHARED 
    src/measurable.cpp
)

set_target_properties(smokyprobe PROPERTIES SOVERSION 1)

target_include_directories(smokyprobe PUBLIC ${PROJECT_SOURCE_DIR}/include)
