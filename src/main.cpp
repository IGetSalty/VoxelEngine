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
    -0.500f, -0.500f, -0.500f, 0.583f, 0.771f, 0.014f,
    -0.500f, -0.500f,  0.500f, 0.609f, 0.115f, 0.436f,
    -0.500f,  0.500f,  0.500f, 0.327f, 0.483f, 0.844f,

     0.500f,  0.500f, -0.500f, 0.822f, 0.569f, 0.201f,
    -0.500f, -0.500f, -0.500f, 0.435f, 0.602f, 0.223f,
    -0.500f,  0.500f, -0.500f, 0.310f, 0.747f, 0.185f,

     0.500f, -0.500f,  0.500f, 0.597f, 0.770f, 0.761f,
    -0.500f, -0.500f, -0.500f, 0.559f, 0.436f, 0.730f,
     0.500f, -0.500f, -0.500f, 0.359f, 0.583f, 0.152f,

     0.500f,  0.500f, -0.500f, 0.483f, 0.596f, 0.789f,
     0.500f, -0.500f, -0.500f, 0.559f, 0.861f, 0.639f,
    -0.500f, -0.500f, -0.500f, 0.195f, 0.548f, 0.859f,

    -0.500f, -0.500f, -0.500f, 0.014f, 0.184f, 0.576f,
    -0.500f,  0.500f,  0.500f, 0.771f, 0.328f, 0.970f,
    -0.500f,  0.500f, -0.500f, 0.406f, 0.615f, 0.116f,

     0.500f, -0.500f,  0.500f, 0.676f, 0.977f, 0.133f,
    -0.500f, -0.500f,  0.500f, 0.971f, 0.572f, 0.833f,
    -0.500f, -0.500f, -0.500f, 0.140f, 0.616f, 0.489f,

    -0.500f,  0.500f,  0.500f, 0.997f, 0.513f, 0.064f,
    -0.500f, -0.500f,  0.500f, 0.945f, 0.719f, 0.592f,
     0.500f, -0.500f,  0.500f, 0.543f, 0.021f, 0.978f,

     0.500f,  0.500f,  0.500f, 0.279f, 0.317f, 0.505f,
     0.500f, -0.500f, -0.500f, 0.167f, 0.620f, 0.077f,
     0.500f,  0.500f, -0.500f, 0.347f, 0.857f, 0.137f,

     0.500f, -0.500f, -0.500f, 0.055f, 0.953f, 0.042f,
     0.500f,  0.500f,  0.500f, 0.714f, 0.505f, 0.345f,
     0.500f, -0.500f,  0.500f, 0.783f, 0.290f, 0.734f,

     0.500f,  0.500f,  0.500f, 0.722f, 0.645f, 0.174f,
     0.500f,  0.500f, -0.500f, 0.302f, 0.455f, 0.848f,
    -0.500f,  0.500f, -0.500f, 0.225f, 0.587f, 0.040f,

     0.500f,  0.500f,  0.500f, 0.517f, 0.713f, 0.338f,
    -0.500f,  0.500f, -0.500f, 0.053f, 0.959f, 0.120f,
    -0.500f,  0.500f,  0.500f, 0.393f, 0.621f, 0.362f,

     0.500f,  0.500f,  0.500f, 0.673f, 0.211f, 0.457f,
    -0.500f,  0.500f,  0.500f, 0.820f, 0.883f, 0.371f,
     0.500f, -0.500f,  0.500f, 0.982f, 0.099f, 0.879f
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

    VertexBuffer vbo(vertices, sizeof(vertices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vao.unbind();

    glEnable(GL_DEPTH_TEST);

    //INPUT

    //UPDATE
    while (!gameWindow.shouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao.getRendererID());
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(gameWindow.getNativeWindow());
        glfwPollEvents();
        
        gameWindow.update();
    }

    //RENDER

}