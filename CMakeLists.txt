cmake_minimum_required(VERSION 3.28)
project(OpenGL_Learning VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

set(GLEW_LINK ${PROJECT_SOURCE_DIR}/dependencies/GL/lib/libGLEW.2.1.0.dylib)
set(GLFW_LINK ${PROJECT_SOURCE_DIR}/dependencies/GLFW/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

set(OPENGL-LEARNING-SRC
        config.h
        main.cpp
)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(OpenGL_Learning ${OPENGL-LEARNING-SRC} ${SRC_FILES})

target_include_directories(OpenGL_Learning PRIVATE dependencies)

target_link_libraries(OpenGL_Learning OpenGL::GL)
# target_link_libraries(OpenGL_Learning "-framework GLUT")
