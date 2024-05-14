cmake_minimum_required(VERSION 3.28)
project(OpenGL_Learning)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_H ${PROJECT_SOURCE_DIR}/dependencies/GL/include)
set(GLFW_H ${PROJECT_SOURCE_DIR}/dependencies/GLFW/include)
include_directories(${GLEW_H} ${GLFW_H})

set(GLEW_LINK ${PROJECT_SOURCE_DIR}/dependencies/GL/lib/libGLEW.2.1.0.dylib)
set(GLFW_LINK ${PROJECT_SOURCE_DIR}/dependencies/GLFW/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

set( OPENGL-LEARNING-SRC
        main.cpp
)

add_executable( OpenGL_Learning ${OPENGL-LEARNING-SRC})

target_link_libraries(OpenGL_Learning "-framework OpenGL")
target_link_libraries(OpenGL_Learning "-framework GLUT")
