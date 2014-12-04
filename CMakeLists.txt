cmake_minimum_required(VERSION 2.8)
project(siriusViz)



find_package( OpenGL REQUIRED )
if (NOT OPENGL_FOUND)
    message(STATUS "WARNING: OPENGL was not found. ${ERR_MSG}")
    return()
endif()


find_package( GLEW REQUIRED )
if (NOT GLEW_FOUND)
    message(STATUS "WARNING: GLEW was not found. ${ERR_MSG}")
    return()
endif()



find_package( GLUT REQUIRED )
if (NOT GLUT_FOUND)
    message(STATUS "WARNING: GLUT was not found. ${ERR_MSG}")
    return()
endif()

#find_package( OpenGL REQUIRED )
#if (NOT OpenGL_FOUND)
#   message(STATUS "WARNING: OpenGL was not found. ${ERR_MSG}")
#   return()
#endif()

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})

add_executable( siriusViz graph.cpp )
target_link_libraries( siriusViz ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} shader_utils utilidades)
add_subdirectory(common)
add_subdirectory(utilities)
