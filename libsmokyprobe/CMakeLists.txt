
cmake_minimum_required(VERSION 2.8)

set (PROJECT_NAME smokyprobe)

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

set(HEADERS "${PROJECT_SOURCE_DIR}/include/smokyprobe.h;${PROJECT_SOURCE_DIR}/include/probe.h;${PROJECT_SOURCE_DIR}/include/probe_uart.h;${PROJECT_SOURCE_DIR}/include/config.h;${PROJECT_SOURCE_DIR}/include/types.h;${PROJECT_SOURCE_DIR}/include/probe_uart_safe.h;${PROJECT_SOURCE_DIR}/include/smart_semaphore.h")

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_link_libraries(${PROJECT_NAME}
	log4cpp
	pthread
	rt
)


install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/smokyprobe
)

install(FILES logprobe.properties 
	DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/
)

