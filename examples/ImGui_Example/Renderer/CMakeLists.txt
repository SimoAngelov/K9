cmake_minimum_required(VERSION 3.7.2)

# Specify C++14 as the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Name of the library
set(LIB "K9_Renderer")
set(${LIB}_include_list "")

# IMPORTANT: EXE_PATH must be set to in the project CMakeLists.txt

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "EXE_PATH: ${EXE_PATH}")

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

add_custom_command(TARGET ${LIB} POST_BUILD        # Adds a post-build event to ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different           # which executes "cmake - E copy_if_different..."
        ${argFileToCopy}                                    # <--this is in-file
        ${EXE_PATH})                        				# <--this is out-file path 
endfunction()

# Function to copy assets based on the provided script
# arg[0] copy_assets - name of the function
# arg[1] - argTarget - target dependency
# arg[2] argCopyScriptPath - path to script that will copy the fo
function(copy_assets argTarget argCopyScriptPath)
if((NOT "${argTarget}" STREQUAL "") AND (NOT "${argCopyScriptPath}" EQUAL ""))
        add_custom_target(${argTarget} 
        COMMAND ${CMAKE_COMMAND} -DASSETS_PATH=${EXE_PATH} -P ${argCopyScriptPath}
    )
    add_dependencies(${LIB} ${argTarget})
    message(STATUS "copy_assets argTarget: ${argTarget} \t argCopyScriptPath: ${argCopyScriptPath}")
endif()
endfunction()

# Function for adding a linking a library in Windows.
# arg[0] - add_lib_windows - name of the function
# arg[1] - argIncludePath - path to the library include directory
# arg[2] - argLibsToAdd - name of the libraries to be added
# arg[3] - argFilesToCopy - files to be copied after build is finished. "" if there're none.
function(add_lib_windows argIncludePath argLibsToAdd argFilesToCopy )

    if(NOT "${argIncludePath}" STREQUAL "")
	    # Add the include directory to the project
	    target_include_directories(${LIB} PUBLIC ${argIncludePath})
		list(APPEND ${LIB}_include_list ${argIncludePath})
    endif()
	
    if(NOT "${argLibsToAdd}" STREQUAL "")
	    # Link the static library
        target_link_libraries(${LIB} PUBLIC ${argLibsToAdd})
    endif()

    if(NOT "${argFilesToCopy}" STREQUAL "")
        # After the buld is finished, copy the argFilesToCopy to the output directory, where the LIB executable is located
        post_build_copy("${argFilesToCopy}")
    endif()
    message(STATUS "[add_lib_windows] ${LIB}\n argIncludePath:  ${argIncludePath} \n argLibsToAdd: ${argLibsToAdd}\n argFilesToCopy: ${argFilesToCopy}")
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

# Get all source files and headers in src/
SOURCE_FILES(source_file_list src)

list(LENGTH source_file_list source_file_list_count)
message(STATUS "[INFO] ${LIB} Found ${source_file_list_count} source files.")

# Create LIB executable
add_library(${LIB} ${source_file_list})

# Add include directories
set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${LIB} PUBLIC ${LIBS})

# Link the source code library OS-specific libraries
if(UNIX)
	#s Setup OpenGL
	#cmake --help-policy CMP0072
	set(OpenGL_GL_PREFERENCE GLVND) 
	find_package(OpenGL REQUIRED)
	target_link_libraries(${LIB} PUBLIC opengl32)
    add_definitions(-DGLFW_INCLUDE_NONE)

    set(LIB_INCLUDE_PATH "")
    set(LIB_TO_ADD "")
    set(FILES_TO_COPY "")

	# Setup SDL
	# apt-get install -y libsdl2-image-2.0-0

	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	set(SDL2_IMAGE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/SDL2/include)
	set(SDL2_IMAGE_LIBRARIES "libSDL2_image-2.0.so.0")
    target_include_directories(${LIB} PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
	target_link_libraries(${LIB} PUBLIC GL ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
	message(STATUS " SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} \n SDL2_LIBRARIES ${SDL2_LIBRARIES}  \n SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIRS} \nSDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES}")


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

    # Copy assets folder to the build directory
    copy_assets("assets" "${CMAKE_CURRENT_SOURCE_DIR}/copy_resources.cmake")
endif(UNIX)
if(WIN32)
	#s Setup OpenGL
	#cmake --help-policy CMP0072
	set(OpenGL_GL_PREFERENCE GLVND) 
	find_package(OpenGL REQUIRED)
	target_link_libraries(${LIB} PUBLIC opengl32)
    add_definitions(-DGLFW_INCLUDE_NONE)

    set(LIB_INCLUDE_PATH "")
    set(LIB_TO_ADD "")
    set(FILES_TO_COPY "")

	# Setup SDL
	add_subdirectory(${LIBS}/SDL2)
    set_lib_sdl2("${LIB_PATH_SDL}" LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY DEPENDENCY_TARGET)
    add_lib_windows("${LIB_INCLUDE_PATH}" "${LIB_TO_ADD}" "${FILES_TO_COPY}")
	reset_lib_variables(LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY)

    # Setup SDL_image
    add_subdirectory(${LIBS}/SDL2_image)
    set_lib_sdl2_image("${LIB_PATH_SDL_IMAGE}" LIB_INCLUDE_PATH LIB_TO_ADD FILES_TO_COPY DEPENDENCY_TARGET)
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

	message(INFO "K9_Renderer_include_list: ${K9_Renderer_include_list}")

    # Copy assets folder to the build directory
    copy_assets("assets" "${CMAKE_CURRENT_SOURCE_DIR}/copy_resources.cmake")
endif(WIN32)
