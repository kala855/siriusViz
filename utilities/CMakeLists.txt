include_directories(${siriusViz_SOURCE_DIR}/utilities)
link_directories(${siriusViz_BINARY_DIR}/utilities)

#building just a library.
add_library(utilidades utilidades.cpp)
target_link_libraries(utilidades ${OpenGL_LIBS} ${GLUT_LIBS} ${GLEW_LIBS})
