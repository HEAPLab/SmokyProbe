
cmake_minimum_required(VERSION 2.8)

set (TARGET_NAME smokyprobe)

#project(${TARGET_NAME})

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_library(log4cpp REQUIRED)

add_definitions(-DLOGFILE="${CMAKE_INSTALL_PREFIX}/etc/logprobe.properties")
message(STATUS "BINARY DIR: " ${CMAKE_PROJECT_BINARY_DIR})

add_subdirectory(src)
message (STATUS "Source files: " ${PROJECT_SRC})

include_directories(include)

add_library(${TARGET_NAME} SHARED
	${PROJECT_SRC}
)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set_target_properties(${TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_link_libraries(${TARGET_NAME}
	log4cpp
	pthread
	rt
)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS ${TARGET_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/smokyprobe
)

install(FILES logprobe.properties 
	DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/
)

