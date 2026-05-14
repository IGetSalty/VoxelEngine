cmake_minimum_required(VERSION 3.20)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)

add_executable(
    VoxelEngine
    src/main.cpp
    src/glad.c
)

target_include_directories(VoxelEngine PRIVATE include)

target_link_libraries(VoxelEngine
    glfw
    GL
)