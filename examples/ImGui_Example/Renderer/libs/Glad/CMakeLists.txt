cmake_minimum_required(VERSION 3.7.2)

add_library(k9_glad
            ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h
        	${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
	        ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
target_include_directories(k9_glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# set the current cmake path
set(LIB_PATH_GLAD ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)

# Function for setting output library variables
# arg[0] - set_k9_lib_glad - name of the function
# arg[1] - inLibPath - input param. Path to library directory
# arg[1] - outIncludePath - output param. Path to the library include directory
# arg[2] - outLibsToAdd - output param. Name of the libraries to be added
# arg[3] - outFilesToCopy - output param. Files to be copied after build is finished. "" if there're none.
function(set_lib_k9_glad inLibPath outIncludePath outLibsToAdd outFilesToCopy)
	# Set the include path
	set(INCLUDE_PATH "${inLibPath}/include")

	# Set the static libraries to be added
	set(LIBS "k9_glad")
	string(STRIP "${LIBS}" LIBS)

	message(STATUS "[set_lib_k9_glad] ${BINARY}\n INCLUDE_PATH ${INCLUDE_PATH}\n LIBS: ${LIBS}\n FILES_TO_COPY:  ${FILES_TO_COPY}")

	#set output variables
	set(${outIncludePath} ${INCLUDE_PATH} PARENT_SCOPE)
	set(${outLibsToAdd} ${LIBS} PARENT_SCOPE)
	set(${outFilesToCopy} ${FILES_TO_COPY} PARENT_SCOPE)
endfunction()