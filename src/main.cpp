#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Window.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"
#include <memory>
#include <iostream>

//Temp Vertex Data to get shaders working
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

int main() {
    //create shaders



    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    Window gameWindow(1200,800,"Test Window");

    VertexShader shader;
    const char* vertexShaderSource = shader.CreateShaderSource('v');
    const char* fragmentShaderSource = shader.CreateShaderSource('f');

    unsigned int vertexShader = 0, fragmentShader = 0;
    vertexShader = shader.BuildShader(vertexShader, vertexShaderSource,'v');
    fragmentShader = shader.BuildShader(fragmentShader, fragmentShaderSource,'f');
    
    unsigned int shaderProgram = shader.CreateShaderProgram(vertexShader, fragmentShader);

    VertexArray vao;
    vao.bind();

    VertexBuffer vbo(vertices,sizeof(vertices));

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //INPUT

    //UPDATE
    while (!gameWindow.shouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao.getRendererID());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(gameWindow.getNativeWindow());
        glfwPollEvents();
        
        gameWindow.update();
    }

    //RENDER

}   