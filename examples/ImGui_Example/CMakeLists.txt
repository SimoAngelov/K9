cmake_minimum_required(VERSION 3.1)

# Name of the project
project(ImGUI_Example)

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

# Function for adding a linking a library in Windows.
# arg[0] - add_lib_windows - name of the function
# arg[1] - argIncludePath - path to the library include directory
# arg[2] - argLibsToAdd - name of the libraries to be added
# arg[3] - argFilesToCopy - files to be copied after build is finished. "" if there're none.
function(add_lib_windows argIncludePath argLibsToAdd argFilesToCopy)
	# Add the include directory to the project
	target_include_directories(${BINARY} PUBLIC ${argIncludePath})
	
	# Link the static library
    target_link_libraries(${BINARY} PUBLIC ${argLibsToAdd})

    if(NOT argFilesToCopy STREQUAL "")
        # After the buld is finished, copy the argFilesToCopy to the output directory, where the binary executable is located
        post_build_copy(${argFilesToCopy})
    endif()
    message(STATUS "[add_lib_windows] ${BINARY}\n argIncludePath:  ${argIncludePath} \n argLibsToAdd: ${argLibsToAdd}\n argFilesToCopy: ${argFilesToCopy}")
endfunction()

# Function for reseting library variables.
# arg[0] - reset_lib_variables - name of the function
# arg[1] - argIncludePath - path to the library include directory
# arg[2] - argLibsToAdd - name of the libraries to be added
# arg[3] - argFilesToCopy - files to be copied after build is finished. "" if there're none.
function(reset_lib_variables argIncludePath argLibsToAdd argFilesToCopy)
	# Reset the parameters
	set(${argIncludePath} "" PARENT_SCOPE)
	set(${argLibsToAdd} "" PARENT_SCOPE)
	set(${argFilesToCopy} "" PARENT_SCOPE)
endfunction()


# Name of the executable
set(BINARY ${PROJECT_NAME})

# Get all source files and headers in src/
SOURCE_FILES(source_file_list src)

list(LENGTH source_file_list source_file_list_count)
message(STATUS "[INFO] ${BINARY} Found ${source_file_list_count} source files.")

# Create BINARY executable
add_executable(${BINARY} ${source_file_list} "src/ImGUIWrapper/ImGUIWrapper.h" "src/ImGUIWrapper/ImGUIWrapper.cpp" "src/Window/Window.h" "src/Window/Window.cpp" "src/MainLoop/MainLoop.h" "src/MainLoop/MainLoop.cpp")

# Add include directories
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
target_include_directories(${BINARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${BINARY} PUBLIC ${LIBS})

# Link the source code library OS-specific libraries
if(UNIX)
    find_package(SDL2 REQUIRED)
    target_link_libraries(${BINARY} PUBLIC SDL2)
endif(UNIX)
if(WIN32)
	#s Setup OpenGL
	#cmake --help-policy CMP0072
	set(OpenGL_GL_PREFERENCE GLVND) 
	find_package(OpenGL REQUIRED)
	target_link_libraries(${BINARY} PUBLIC opengl32)

    set(LIB_INCLUDE_PATH "")
    set(LIB_TO_ADD "")
    set(FILES_TO_COPY "")

	# Setup SDL
	add_subdirectory(${LIBS}/SDL2)
    set_lib_sdl2("${LIB_PATH_SDL}" LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)
    add_lib_windows("${LIB_INCLUDE_PATH}" "${LIB_TO_ADD}" "${FILES_TO_COPY}")
	reset_lib_variables(LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)

	# Setup Glad
	add_subdirectory(${LIBS}/Glad)
    set_lib_glad("${LIB_PATH_GLAD}" LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)
    add_lib_windows("${LIB_INCLUDE_PATH}" "${LIB_TO_ADD}" "${FILES_TO_COPY}")
	reset_lib_variables(LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)

	# Setup GLM
	add_subdirectory(${LIBS}/glm)
    set_lib_glm("${LIB_PATH_GLM}" LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)
    add_lib_windows("${LIB_INCLUDE_PATH}" "${LIB_TO_ADD}" "${FILES_TO_COPY}")
	reset_lib_variables(LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)

	# Setup ImGUI
	add_subdirectory(${LIBS}/imgui)
    set_lib_imgui("${LIB_PATH_IMGUI}" LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)
    add_lib_windows("${LIB_INCLUDE_PATH}" "${LIB_TO_ADD}" "${FILES_TO_COPY}")
	reset_lib_variables(LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)

endif(WIN32)