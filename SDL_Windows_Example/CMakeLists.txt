cmake_minimum_required(VERSION 3.1)

# Name of the project
project(SDL_Test)

message(STATUS "${PROJECT_NAME} is located in ${CMAKE_CURRENT_SOURCE_DIR}")

# Specify C++14 as the standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")


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

# Macro for gathering all include paths recursively
# arg[0] The macro name
# arg[1] Output variable where all the include paths will be written
# arg[2] Current path from which to start looking recursively
MACRO(HEADER_DIRECTORIES return_list curr_dir)
    FILE(GLOB_RECURSE new_list ${curr_dir}/*.h ${curr_dir}/*.hpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# Macro for gathering all source files recursively
# arg[0] The macro name
# arg[1] Output variable where all the source files will be written
# arg[2] Current path from which to start looking recursively
MACRO(SOURCE_FILES return_list curr_dir)
    FILE(GLOB_RECURSE new_list ${curr_dir}/*.h ${curr_dir}/*.cpp ${curr_dir}/*.hpp ${curr_dir}/*.inl)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        message(STATUS "[INFO] file_path: ${file_path}")
        SET(dir_list ${dir_list} ${file_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# Function for copying the specified file path to the output directory where the executable is located
# arg[0] - post_build_copy - name of the function
# arg[1] - argFileToCopy - file to be copied
function(post_build_copy argFileToCopy)
    message(STATUS "[INFO]\n\n\n post_build_copy  ${PROJECT_NAME} \n\t\t copying argFileToCopy: ${argFileToCopy}\n\t\t to CMAKE_CURRENT_BINARY_DIR:  ${CMAKE_CURRENT_BINARY_DIR}\n")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different           # which executes "cmake - E copy_if_different..."
        ${argFileToCopy}                                    # <--this is in-file
        ${CMAKE_CURRENT_BINARY_DIR})                        # <--this is out-file path 
endfunction()

# Name of the executable
set(BINARY ${PROJECT_NAME})

# Get all source files and headers in src/
SOURCE_FILES(source_file_list src)

list(LENGTH source_file_list source_file_list_count)
message(STATUS "[INFO] ${BINARY} Found ${source_file_list_count} source files.")

# Create BINARY executable
add_executable(${BINARY} ${source_file_list})

# Add include directories
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${LIBS})

# Link the source code library OS-specific libraries
if(UNIX)
    find_package(SDL2 REQUIRED)
    target_link_libraries(${BINARY} PUBLIC SDL2)
endif(UNIX)
if(WIN32)
	# Setup SDL
	# Set the SDL library path
    set(SDL_PATH ${LIBS}/SDL2)
	
	# Include and execute the SDL CmakeLists.txt
	add_subdirectory(${SDL_PATH})

	# Add the SDL include directory to the project
	target_include_directories(${BINARY} PUBLIC ${SDL2_INCLUDE_DIRS})
	
	# Link the SDL static libraries
    target_link_libraries(${BINARY} PUBLIC ${SDL2_LIBRARIES})
	
	# After the buld is finished, copy the SDL dll to the output directory, where the binary executable is located
    post_build_copy("${EXTERN_SDL_LIBS_PATH}/SDL2.dll")
    message(STATUS "[INFO] ${BINARY} SDL_PATH: ${SDL_PATH}\n SDL_INCLUDE_DIRS:  ${SDL2_INCLUDE_DIRS} \n SDL2_LIBRARIES: ${SDL2_LIBRARIES} \n EXTERN_SDL_LIBS_PATH: ${EXTERN_SDL_LIBS_PATH} ")
endif(WIN32)