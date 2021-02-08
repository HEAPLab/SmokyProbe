cmake_minimum_required(VERSION 3.0)

## Use all the *.cpp files we found under this folder for the project
FILE (GLOB SRCS "*.cpp" "*.c")

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS}")

set (LIBSMOKYPROBE_PATH ${CMAKE_INSTALL_PREFIX})

find_library(LIBSMOKYPROBE smokyprobe
	HINTS ${LIBSMOKYPROBE_PATH}
)

include_directories (${LIBSMOKYPROBE_PATH}/include)

link_directories (${LIBSMOKYPROBE_PATH}/lib)

# Tools
add_subdirectory(monitor)
add_subdirectory(energy)
add_subdirectory(readivp)


