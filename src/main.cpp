#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/Window.h"
#include "math/Camera.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"
#include "util/FileHandling.h"
#include "world/Block.h"
//#include <memory>
#include <iostream>
#include <string>
//#include <fstream>
//#include <sstream>
#include <vector>

//this is a test for committ.


//Temp Vertex Data to get shaders working
static const GLfloat vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};


static const GLfloat colours[] = {
    0.90f, 0.10f, 0.10f,//  1  - Red
    0.10f, 0.90f, 0.10f,//  2  - Green
    0.10f, 0.10f, 0.90f,//  3  - Blue
    0.10f, 0.70f, 0.90f,//  4  - Cyan
    0.90f, 0.30f, 0.70f,//  5  - Pink
    0.90f, 0.70f, 0.10f,//  6  - Yellow
    0.10f, 0.90f, 0.70f,//  7  - Turquoise
    0.40f, 0.30f, 0.20f,//  8  - Brown
    0.10f, 0.30f, 0.70f,//  9  - Dark Blue
    0.40f, 0.60f, 0.10f,// 10  - Lime
    0.80f, 0.10f, 0.90f,// 11  - Purple
    0.90f, 0.70f, 0.80f,// 12  - Pale Pink
    0.80f, 0.90f, 0.10f,// 13  - Yellow-Green
    0.70f, 0.20f, 0.30f,// 14  - Crimson
    0.90f, 0.50f, 0.30f,// 15  - Orange
    0.10f, 0.50f, 0.40f,// 16  - Teal
    0.90f, 0.90f, 0.60f,// 17  - Pale Yellow
    0.10f, 0.30f, 0.40f,// 18  - Dark Teal
    0.10f, 0.30f, 0.90f,// 19  - Bright Blue
    0.50f, 0.10f, 0.60f,// 20  - Dark Purple
    0.10f, 0.90f, 0.90f,// 21  - Bright Cyan
    0.10f, 0.80f, 0.40f,// 22  - Emerald
    0.50f, 0.30f, 0.50f,// 23  - Mauve
    0.70f, 0.50f, 0.90f,// 24  - Lavender
    0.10f, 0.50f, 0.80f,// 25  - Sky Blue
    0.90f, 0.10f, 0.30f,// 26  - Red-Pink
    0.60f, 0.50f, 0.10f,// 27  - Olive
    0.90f, 0.10f, 0.50f,// 28  - Magenta
    0.50f, 0.80f, 0.10f,// 29  - Bright Lime
    0.60f, 0.10f, 0.40f,// 30  - Dark Magenta
    0.70f, 0.20f, 0.10f,// 31  - Dark Orange
    0.90f, 0.50f, 0.10f,// 32  - Orange-Yellow
    0.60f, 0.30f, 0.90f,// 33  - Violet
    0.80f, 0.80f, 0.30f,// 34  - Khaki    
    0.90f, 0.10f, 0.80f,// 35  - Hot Pink
    0.80f, 0.60f, 0.50f // 36  - Peach
};


void GenerateBlocks(std::vector<Block>& Blocks) {

    int width = 100;
    int height = 100;
    int depth = 100;
    
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < depth; ++k) {
                Blocks.emplace_back(
                    glm::vec3(i * 2.0f, j * 2.0f, k * 2.0f),
                    glm::mat4(1.0f)
                );
            }
        }
    };
}




int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    //Create Game Window/Camera/Shaders and Create Blocks/Models for Blocks and reserve space
    Window gameWindow(1920,1080,"Test Window"); //GameWindow
    Camera buddy; 
    Shader shader; 

    std::vector<Block> Blocks;
    Blocks.reserve(100*100*100);
    GenerateBlocks(Blocks);

    //Creating model matrix for each block in Blocks
    for (Block& block : Blocks) {
        block.model = glm::translate(
                            glm::mat4(1.0f), 
                            block.position
                        );
        //std::cout << sizeof(block);
    }

    //Instanced Rendering: Upload block positions or model matrics to GPU Buffer. Use glDrawArraysInstanced() to render
    //many copies of the same cube in a single draw call
    std::vector<glm::vec3> positions;
    for (const Block& block : Blocks) {
        positions.push_back(block.position);
    }
    
    //Creating Shader Program
    const std::string vertexShaderSourceString = ReadTextFile("src/graphics/shaders/vertexShader.glsl");
    const std::string fragmentShaderSourceString = ReadTextFile("src/graphics/shaders/fragmentShader.glsl");

    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

    unsigned int vertexShader = 0, fragmentShader = 0;
    vertexShader = shader.BuildShader(vertexShader, vertexShaderSource, 'v');
    fragmentShader = shader.BuildShader(fragmentShader, fragmentShaderSource, 'f');

    unsigned int shaderProgram = shader.CreateShaderProgram(vertexShader, fragmentShader);

    //Generate and bind the VAO
    VertexArray vao;
    vao.bind(); 

    //Creating Buffer objects - Note 0 = Location 0 and 1 = Location 1 in glsl
    //POSITION 
    VertexBuffer vbo;
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    //COLOUR 
    VertexBuffer cbo;
    cbo.bind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(colours),colours,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);

    //INSTANCE
    VertexBuffer ibo;
    ibo.bind();
    glBufferData(GL_ARRAY_BUFFER,positions.size()*sizeof(glm::vec3), positions.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2,1);
    

    //Locating Camera Uniforms, telling GPU to use shaderProgram, and setting Camera Projection 
    //as it does not change

    //CAMERA -- Locating each of the uniforms in the shader so the program can use it
    GLint modelLoc  = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc   = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc   = glGetUniformLocation(shaderProgram, "projection");


    glUseProgram(shaderProgram); //Why does this need to go here?
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(buddy.cameraProjection)); 

    vao.unbind(); 

    //[LEARN LATER]
    glEnable(GL_DEPTH_TEST);


    while (!gameWindow.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

        //Does exactly what it says, clears the buffers at the start of each frame.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Remember when we created that shader program?
        glUseProgram(shaderProgram); //Good practice to call again unless i can be sure it'll persist.

        //Updates Camera Position and refreshes Uniform
        buddy.cameraView = buddy.updateCameraPosition(gameWindow.getNativeWindow());
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(buddy.cameraView)); //View Matrix upload.

        //Activates VAO
        glBindVertexArray(vao.getRendererID());

        //Block Model Matrix Upload
        //for (Block& block : Blocks) { 
        //    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(block.model));
        //    glDrawArrays(GL_TRIANGLES, 0, 36); 
        //}
        
        glDrawArraysInstanced(GL_TRIANGLES,0,36,static_cast<GLsizei>(Blocks.size()));

        //Implements double buffering (by swapping front and back buffers)
        glfwSwapBuffers(gameWindow.getNativeWindow());
        glfwPollEvents();

        gameWindow.update();
    }
    }