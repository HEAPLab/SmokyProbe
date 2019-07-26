cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set (PROJECT_NAME "smokytest" )

set (LIBSMOKYPROBE_UART_PATH ${CMAKE_SOURCE_DIR}/../../libsmokyprobe-dev/)
message ("DEV PATH = " ${LIBSMOKYPROBE_UART_PATH})

## Set our project name
project (${PROJECT_NAME})

## Use all the *.cpp files we found under this folder for the project
FILE (GLOB SRCS "*.cpp" "*.c")

set (CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall ${CMAKE_CXX_FLAGS}")


include_directories (
	${LIBSMOKYPROBE_UART_PATH}/include
)

link_directories (
	${LIBSMOKYPROBE_UART_PATH}/build/Debug
)

## Define the executable
add_executable (${PROJECT_NAME} ${SRCS})

target_link_libraries (${PROJECT_NAME}
	smokyprobe-uart
	log4cpp
	pthread
)

