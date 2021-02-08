cmake_minimum_required(VERSION 3.0)

set (TARGET_NAME "smokytools-energy" )

message (STATUS ${PROJECT_NAME}": " ${TARGET_NAME})

file (GLOB SRCS "*.cpp" "*.c")

add_executable (${TARGET_NAME} ${SRCS})

target_link_libraries (${TARGET_NAME}
	smokyprobe
	log4cpp
	pthread
)

set_target_properties (${TARGET_NAME} PROPERTIES
	INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
	INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
