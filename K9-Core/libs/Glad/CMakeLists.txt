add_library(glad
            ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h
        	${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
	        ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
