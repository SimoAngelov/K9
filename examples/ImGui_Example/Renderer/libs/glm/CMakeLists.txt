cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

set(GLM_VERSION "0.9.9")
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)
enable_testing()

add_subdirectory(glm)
add_library(glm::glm ALIAS glm)
add_library(k9_glm ALIAS glm)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)

endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})


# set the current cmake path
set(LIB_PATH_GLM ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)

# Function for setting output library variables
# arg[0] - set_lib_k9_glm - name of the function
# arg[1] - inLibPath - input param. Path to library directory
# arg[1] - outIncludePath - output param. Path to the library include directory
# arg[2] - outLibsToAdd - output param. Name of the libraries to be added
# arg[3] - outFilesToCopy - output param. Files to be copied after build is finished. "" if there're none.
function(set_lib_k9_glm inLibPath outIncludePath outLibsToAdd outFilesToCopy)
	# Set the include path
	set(INCLUDE_PATH "${inLibPath}/glm")

	# Set the static libraries to be added
	set(LIBS "k9_glm")
	string(STRIP "${LIBS}" LIBS)

	message(STATUS "[set_lib_k9_glm] ${BINARY}\n INCLUDE_PATH ${INCLUDE_PATH}\n LIBS: ${LIBS}\n FILES_TO_COPY:  ${FILES_TO_COPY}")

	#set output variables
	set(${outIncludePath} ${INCLUDE_PATH} PARENT_SCOPE)
	set(${outLibsToAdd} ${LIBS} PARENT_SCOPE)
	set(${outFilesToCopy} ${FILES_TO_COPY} PARENT_SCOPE)
endfunction()