cmake_minimum_required(VERSION 3.7.2)

# set the current cmake path
set(LIB_PATH_SDL ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)

# Function for setting output library variables
# arg[0] - set_lib_k9_sdl2 - name of the function
# arg[1] - inLibPath - input param. Path to library directory
# arg[1] - outIncludePath - output param. Path to the library include directory
# arg[2] - outLibsToAdd - output param. Name of the libraries to be added
# arg[3] - outFilesToCopy - output param. Files to be copied after build is finished. "" if there're none.
function(set_lib_k9_sdl2 inLibPath outIncludePath outLibsToAdd outFilesToCopy)
	# Set the include path
	set(INCLUDE_PATH "${inLibPath}/include")

	# Support both 32 and 64 bit builds
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
	  set(LIBS_PATH "${inLibPath}/lib/x64")
	else ()
	  set(LIBS_PATH "${inLibPath}/lib/x86")
	endif ()

	# Set the static libraries to be added
	set(LIBS "${LIBS_PATH}/SDL2.lib;${LIBS_PATH}/SDL2main.lib")
	string(STRIP "${LIBS}" LIBS)

	# Set files to be copied after the build is finished
	set(FILES_TO_COPY "${LIBS_PATH}/SDL2.dll")

	message(STATUS "[set_lib_k9_sdl2] ${BINARY}\n INCLUDE_PATH ${INCLUDE_PATH}\n LIBS: ${LIBS}\n FILES_TO_COPY:  ${FILES_TO_COPY} \n\n")

	#set output variables
	set(${outIncludePath} ${INCLUDE_PATH} PARENT_SCOPE)
	set(${outLibsToAdd} ${LIBS} PARENT_SCOPE)
	set(${outFilesToCopy} ${FILES_TO_COPY} PARENT_SCOPE)
endfunction()