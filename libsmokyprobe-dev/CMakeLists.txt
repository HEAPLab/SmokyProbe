
cmake_minimum_required(VERSION 2.8)

set (PROJECT_NAME smokyprobe-uart)

project(${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_library(log4cpp REQUIRED)

add_definitions(-DLOGFILE="${CMAKE_INSTALL_PREFIX}/etc/logprobe.properties")
message(STATUS "PROJECT BINARY DIR: " ${CMAKE_PROJECT_BINARY_DIR})

add_subdirectory(src)
message (STATUS "Source files: " ${PROJECT_SRC})

include_directories(include)

add_library(${PROJECT_NAME} SHARED
	${PROJECT_SRC}
)

target_link_libraries(${PROJECT_NAME}
	log4cpp
	pthread
	rt
)

install(FILES logprobe.properties 
	DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/)
