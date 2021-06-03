cmake_minimum_required(VERSION 3.7.2)
add_library(k9_imgui
    		${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h
			${CMAKE_CURRENT_SOURCE_DIR}/imgui.h
			${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h
			${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/imstb_rectpack.h
			${CMAKE_CURRENT_SOURCE_DIR}/imstb_textedit.h
			${CMAKE_CURRENT_SOURCE_DIR}/imstb_truetype.h
			${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp)

# set the current cmake path
set(LIB_PATH_IMGUI ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)

# Function for setting output library variables
# arg[0] - set_lib_k9_imgui - name of the function
# arg[1] - inLibPath - input param. Path to library directory
# arg[1] - outIncludePath - output param. Path to the library include directory
# arg[2] - outLibsToAdd - output param. Name of the libraries to be added
# arg[3] - outFilesToCopy - output param. Files to be copied after build is finished. "" if there're none.
function(set_lib_k9_imgui inLibPath outIncludePath outLibsToAdd outFilesToCopy)
	# Set the include path
	set(INCLUDE_PATH ${inLibPath})

	# Set the static libraries to be added
	set(LIBS "k9_imgui")
	string(STRIP "${LIBS}" LIBS)

	message(STATUS "[set_lib_k9_imgui] ${BINARY}\n INCLUDE_PATH ${INCLUDE_PATH}\n LIBS: ${LIBS}\n FILES_TO_COPY:  ${FILES_TO_COPY}")

	#set output variables
	set(${outIncludePath} ${INCLUDE_PATH} PARENT_SCOPE)
	set(${outLibsToAdd} ${LIBS} PARENT_SCOPE)
	set(${outFilesToCopy} ${FILES_TO_COPY} PARENT_SCOPE)
endfunction()
