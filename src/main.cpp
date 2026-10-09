#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Window.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"
#include <memory>
#include <iostream>

//Temp Vertex Data to get shaders working
static const GLfloat vertices[] = {
    -0.5f,-0.5f,-0.5f, // triangle 1 : begin
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
    0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};


static const GLfloat colours[] = {

    //  1  - Red
    0.90f, 0.10f, 0.10f,

    //  2  - Green
    0.10f, 0.90f, 0.10f,

    //  3  - Blue
    0.10f, 0.10f, 0.90f,

    //  4  - Cyan
    0.10f, 0.70f, 0.90f,

    //  5  - Pink
    0.90f, 0.30f, 0.70f,

    //  6  - Yellow
    0.90f, 0.70f, 0.10f,

    //  7  - Turquoise
    0.10f, 0.90f, 0.70f,

    //  8  - Brown
    0.40f, 0.30f, 0.20f,

    //  9  - Dark Blue
    0.10f, 0.30f, 0.70f,

    // 10  - Lime
    0.40f, 0.60f, 0.10f,

    // 11  - Purple
    0.80f, 0.10f, 0.90f,

    // 12  - Pale Pink
    0.90f, 0.70f, 0.80f,

    // 13  - Yellow-Green
    0.80f, 0.90f, 0.10f,

    // 14  - Crimson
    0.70f, 0.20f, 0.30f,

    // 15  - Orange
    0.90f, 0.50f, 0.30f,

    // 16  - Teal
    0.10f, 0.50f, 0.40f,

    // 17  - Pale Yellow
    0.90f, 0.90f, 0.60f,

    // 18  - Dark Teal
    0.10f, 0.30f, 0.40f,

    // 19  - Bright Blue
    0.10f, 0.30f, 0.90f,

    // 20  - Dark Purple
    0.50f, 0.10f, 0.60f,

    // 21  - Bright Cyan
    0.10f, 0.90f, 0.90f,

    // 22  - Emerald
    0.10f, 0.80f, 0.40f,

    // 23  - Mauve
    0.50f, 0.30f, 0.50f,

    // 24  - Lavender
    0.70f, 0.50f, 0.90f,

    // 25  - Sky Blue
    0.10f, 0.50f, 0.80f,

    // 26  - Red-Pink
    0.90f, 0.10f, 0.30f,

    // 27  - Olive
    0.60f, 0.50f, 0.10f,

    // 28  - Magenta
    0.90f, 0.10f, 0.50f,

    // 29  - Bright Lime
    0.50f, 0.80f, 0.10f,

    // 30  - Dark Magenta
    0.60f, 0.10f, 0.40f,

    // 31  - Dark Orange
    0.70f, 0.20f, 0.10f,

    // 32  - Orange-Yellow
    0.90f, 0.50f, 0.10f,

    // 33  - Violet
    0.60f, 0.30f, 0.90f,

    // 34  - Khaki
    0.80f, 0.80f, 0.30f,

    // 35  - Hot Pink
    0.90f, 0.10f, 0.80f,

    // 36  - Peach
    0.80f, 0.60f, 0.50f
};

int main() {
    //create shaders



    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    Window gameWindow(1200,800,"Test Window");

    VertexShader shader;
    //Creating Shader Program: In order to create shader program, we need to first create the vertex
    //and fragment shader sources and shaders, and then create the program via CreateShaderProgram.
    //The Shader sources include the position and colour attributes which are then passed as whole 
    //arguments into the BuildShader function which then compiles the shaders and returns the shader IDs. 
    //These are then passed into the CreateShaderProgram function which links the shaders into a program 
    //and returns the program ID. 

    //So we're not dealing with objects themselves just the IDs of objects so OpenGL knows what to do.
    const char* vertexShaderSource = shader.CreateShaderSource('v');
    const char* fragmentShaderSource = shader.CreateShaderSource('f');

    unsigned int vertexShader = 0, fragmentShader = 0;
    vertexShader = shader.BuildShader(vertexShader, vertexShaderSource,'v');
    fragmentShader = shader.BuildShader(fragmentShader, fragmentShaderSource,'f');
    
    unsigned int shaderProgram = shader.CreateShaderProgram(vertexShader, fragmentShader);

    //Here i am telling OpenGL "Hey listen up i've got shit for you to do" and then OpenGL Will record
    //what we need it to do and where it can find different stuff until i vao.unbind() which tells it to
    //stop recording and then we can use the vao to tell OpenGL to do what we told it to do when we 
    //vao.getRendererID() and glBindVertexArray(vao.getRendererID()) and then OpenGL will do what we told 
    //it to do.
    VertexArray vao;
    vao.bind();

    VertexBuffer vbo(vertices, sizeof(vertices));

    // POSITION
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    // COLOUR
    GLuint colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(colours),colours,GL_STATIC_DRAW);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);

    vao.unbind();

    glEnable(GL_DEPTH_TEST);

    while (!gameWindow.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(vao.getRendererID());

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(gameWindow.getNativeWindow());
        glfwPollEvents();

        gameWindow.update();
    }
    }