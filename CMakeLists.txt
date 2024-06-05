cmake_minimum_required(VERSION 3.28)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(OpenGL_Learning VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(OPENGL-LEARNING-SRC
        src/Application.cpp
)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE VENDORS_FILES "vendors/*.cpp")

add_executable(OpenGL_Learning
        ${OPENGL-LEARNING-SRC}
        ${SRC_FILES}
        ${VENDORS_FILES}
)

include_directories(src)

find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE
                                      OpenGL::GL
                                      glfw
                                      glad::glad
                                      imgui::imgui
                                      glm::glm-header-only
)

target_include_directories(OpenGL_Learning PRIVATE "${PROJECT_SOURCE_DIR}/vendors")

target_compile_definitions(OpenGL_Learning PRIVATE WINDOW_WIDTH=1280.0f WINDOW_HEIGHT=720.0f)