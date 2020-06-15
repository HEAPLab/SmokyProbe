cmake_minimum_required(VERSION 3.0)

## Use the variable PROJECT_NAME for changing the target name
set (PROJECT_NAME "smokytools" )

set (LIBSMOKYPROBE_PATH /opt/smokyprobe/)
message ("DEV PATH = " ${LIBSMOKYPROBE_PATH})

## Set our project name
project (${PROJECT_NAME})

## Use all the *.cpp files we found under this folder for the project
FILE (GLOB SRCS "*.cpp" "*.c")

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS}")

find_library(LIBSMOKYPROBE smokyprobe
	HINTS ${LIBSMOKYPROBE_PATH}
)

include_directories (${LIBSMOKYPROBE_PATH}/include)

link_directories (${LIBSMOKYPROBE_PATH}/lib)

# Tools
add_subdirectory(monitor)


