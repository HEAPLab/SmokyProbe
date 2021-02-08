
cmake_minimum_required(VERSION 3.0)

## Set our project name
set (PROJECT_NAME "smokyprobe")
project (${PROJECT_NAME})

add_subdirectory(libsmokyprobe)
add_subdirectory(tools)
