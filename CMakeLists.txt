cmake_minimum_required(VERSION 3.1)

# Specify compilers
if ("${CMAKE_C_COMPILER}" STREQUAL "")
    set(CMAKE_C_COMPILER clang-12 CACHE INTERNAL "")
    message(STATUS "No C compiler was specified. Setting to ${CMAKE_C_COMPILER}")
endif()
if ("${CMAKE_CXX_COMPILER}" STREQUAL "")
    set(CMAKE_CXX_COMPILER clang++-12 CACHE INTERNAL "")
    message(STATUS "No C++ compiler was specified. Setting to ${CMAKE_CXX_COMPILER}")
endif()

# Name of the project
project(K9)

# Specify linker flags - set CMAKE_CURRENT_BINARY_DIR to append the current build directory
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN'" )
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN'" )

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# Check if the build directory starts with build* prefix
if(NOT ${CMAKE_CURRENT_BINARY_DIR} MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/build*")
    message(FATAL_ERROR "\n CMake needs to be run in a directory with prefix /build*")
endif()

# Output compiler Information and build type
message(STATUS "====== Current compiler is: ${CMAKE_CXX_COMPILER} ======")
message(STATUS "====================================")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "No selected build type")
    message(STATUS "Setting to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
else()
    message(STATUS "Selected build type is ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "====================================")

# Get all source files and headers
file(GLOB SOURCES
    src/*.h
    src/*.cpp)

# Create executable PROJECT_NAME
add_executable(${PROJECT_NAME} ${SOURCES})


# Specify C++14 as the standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)


# Add include directories
set(K9_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INTERNAL_DIRS
    ${K9_ROOT_DIR}
    ${K9_ROOT_DIR}/src
    ${K9_ROOT_DIR}/libs)

include_directories(${INTERNAL_DIRS})

# Find required libraries
find_package(OpenGL REQUIRED)

#Link libraries
target_link_libraries(${PROJECT_NAME} GL SDL2)
