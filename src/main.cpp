#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Window.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include <memory>
#include <iostream>

//Temp Vertex Data to get shaders working
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    Window gameWindow(1200,800,"Test Window");

    VertexArray vao;
    vao.bind();

    VertexBuffer vbo(vertices,sizeof(vertices));

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glEnableVertexAttribArray(0);
    //INPUT

    //UPDATE
    while (!gameWindow.shouldClose()) {
        gameWindow.update();

    }

    //RENDER

}