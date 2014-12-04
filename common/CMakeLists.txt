include_directories(${siriusViz_SOURCE_DIR}/common)
link_directories(${siriusViz_BINARY_DIR}/common)

#building just a library.
add_library(shader_utils shader_utils.cpp)
target_link_libraries(shader_utils ${OpenGL_LIBS} ${GLUT_LIBS} ${GLEW_LIBS})
