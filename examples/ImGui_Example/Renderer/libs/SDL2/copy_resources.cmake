cmake_minimum_required(VERSION 3.7.2)

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
	set(LIBS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64")
else ()
	set(LIBS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/x86")
endif ()

file(COPY ${LIBS_PATH}/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})