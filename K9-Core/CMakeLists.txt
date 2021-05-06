cmake_minimum_required(VERSION 3.1)

# Name of the library
set(BINARY ${PROJECT_NAME}-Core)

# Get all source files and headers
SOURCE_FILES(source_dir_list src)

list(LENGTH source_dir_list source_dir_list_count)
message(STATUS "[INFO] ${BINARY} Found ${source_dir_list_count} source files.")

# Create library BINARY
add_library(${BINARY} ${source_dir_list})

# Specify C++14 as the standard
set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD 14)

# Add include directories
set(K9_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIRS
    ${K9_ROOT_DIR}
    ${K9_ROOT_DIR}/src)
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(LIBS_DIRS
    ${LIBS}/
    ${LIBS}/Glad/include
    ${LIBS}/glm/include
    ${LIBS}/imgui
    ${LIBS}/spdlog/include
    ${LIBS}/stb_image)


include_directories(${INTERNAL_DIRS})
include_directories(${LIBS_DIRS})

# Find required libraries
#cmake --help-policy CMP0072
set(OpenGL_GL_PREFERENCE GLVND) 
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

#Link libraries
add_subdirectory(${LIBS}/Glad)
add_subdirectory(${LIBS}/glm)
add_subdirectory(${LIBS}/imgui)
add_subdirectory(${LIBS}/spdlog)
add_subdirectory(${LIBS}/stb_image)

target_link_libraries(${BINARY} GL SDL2 glad glm imgui spdlog stb_image)
