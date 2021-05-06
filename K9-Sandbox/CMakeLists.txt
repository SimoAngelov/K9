cmake_minimum_required(VERSION 3.1)

# Name of the sandbox
set(BINARY ${PROJECT_NAME}-Sandbox)

# Get all source files and headers
SOURCE_FILES(source_file_list src)

message(STATUS "[INFO] ${BINARY} Found ${source_file_list} source files.")

# Create library BINARY
add_executable(${BINARY} ${source_file_list})

# Specify C++14 as the standard
set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD 14)

set(LIB ../${PROJECT_NAME}-Core)
set(LIBS_DIRS
    ${LIB}/src
    ${LIB}/libs
    ${LIB}/libs/Glad/include
    ${LIB}/libs/glm/include
    ${LIB}/libs/imgui
    ${LIB}/libs/spdlog/include)
include_directories(${LIBS_DIRS})

target_link_libraries(${BINARY} PUBLIC ${PROJECT_NAME}-Core)
