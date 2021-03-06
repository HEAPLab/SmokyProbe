cmake_minimum_required(VERSION 3.0)

set (TARGET_NAME smokyutils)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

message (STATUS ${PROJECT_NAME}": " ${TARGET_NAME})

file (GLOB SRCS "*.cpp" "*.c")

add_library (${TARGET_NAME} SHARED ${SRCS})

set_target_properties(${TARGET_NAME} PROPERTIES 
	PUBLIC_HEADER "common.h")

set_target_properties (${TARGET_NAME} PROPERTIES
	INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
	INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS ${TARGET_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/smokyprobe/util
)
