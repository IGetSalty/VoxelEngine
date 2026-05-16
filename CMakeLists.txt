cmake_minimum_required(VERSION 3.20)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)

add_executable(
    VoxelEngine
    src/main.cpp
    src/glad.c
    include/core/Application.h
    include/core/Input.h
    include/core/Window.h
    include/graphics/VertexBuffer.h
    include/graphics/Shader.h
    include/graphics/IndexBuffer.h
    include/graphics/VertexArray.h
    include/graphics/Renderer.h
    include/graphics/Texture.h
    include/math/Camera.h
    include/world/Block.h
    include/world/Chunk.h
    include/world/World.h
    include/util/Timer.h
    src/core/Window.cpp
)

target_include_directories(VoxelEngine PRIVATE include)

target_link_libraries(VoxelEngine
    glfw
    GL
)