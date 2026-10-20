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

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using clock_type = std::chrono::steady_clock;


// Temp Vertex Data to get shaders working
/*
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
*/


static const GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f, // 0
     0.5f, -0.5f, -0.5f, // 1
     0.5f,  0.5f, -0.5f, // 2
    -0.5f,  0.5f, -0.5f, // 3
    -0.5f, -0.5f,  0.5f, // 4
     0.5f, -0.5f,  0.5f, // 5
     0.5f,  0.5f,  0.5f, // 6
    -0.5f,  0.5f,  0.5f  // 7
};

unsigned int indices[] = {
    0, 1, 2, 2, 3, 0, // back
    4, 6, 5, 6, 4, 7, // front (winding must be consistent)
    0, 4, 5, 5, 1, 0,
    3, 2, 6, 6, 7, 3,
    0, 3, 7, 7, 4, 0,
    1, 5, 6, 6, 2, 1
};

static const GLfloat colours[] = {
    0.90f, 0.10f, 0.10f, //  1 - Red
    0.10f, 0.90f, 0.10f, //  2 - Green
    0.10f, 0.10f, 0.90f, //  3 - Blue
    0.10f, 0.70f, 0.90f, //  4 - Cyan
    0.90f, 0.30f, 0.70f, //  5 - Pink
    0.90f, 0.70f, 0.10f, //  6 - Yellow
    0.10f, 0.90f, 0.70f, //  7 - Turquoise
    0.40f, 0.30f, 0.20f, //  8 - Brown
    0.10f, 0.30f, 0.70f, //  9 - Dark Blue
    0.40f, 0.60f, 0.10f, // 10 - Lime
    0.80f, 0.10f, 0.90f, // 11 - Purple
    0.90f, 0.70f, 0.80f, // 12 - Pale Pink
    0.80f, 0.90f, 0.10f, // 13 - Yellow-Green
    0.70f, 0.20f, 0.30f, // 14 - Crimson
    0.90f, 0.50f, 0.30f, // 15 - Orange
    0.10f, 0.50f, 0.40f, // 16 - Teal
    0.90f, 0.90f, 0.60f, // 17 - Pale Yellow
    0.10f, 0.30f, 0.40f, // 18 - Dark Teal
    0.10f, 0.30f, 0.90f, // 19 - Bright Blue
    0.50f, 0.10f, 0.60f, // 20 - Dark Purple
    0.10f, 0.90f, 0.90f, // 21 - Bright Cyan
    0.10f, 0.80f, 0.40f, // 22 - Emerald
    0.50f, 0.30f, 0.50f, // 23 - Mauve
    0.70f, 0.50f, 0.90f, // 24 - Lavender
    0.10f, 0.50f, 0.80f, // 25 - Sky Blue
    0.90f, 0.10f, 0.30f, // 26 - Red-Pink
    0.60f, 0.50f, 0.10f, // 27 - Olive
    0.90f, 0.10f, 0.50f, // 28 - Magenta
    0.50f, 0.80f, 0.10f, // 29 - Bright Lime
    0.60f, 0.10f, 0.40f, // 30 - Dark Magenta
    0.70f, 0.20f, 0.10f, // 31 - Dark Orange
    0.90f, 0.50f, 0.10f, // 32 - Orange-Yellow
    0.60f, 0.30f, 0.90f, // 33 - Violet
    0.80f, 0.80f, 0.30f, // 34 - Khaki
    0.90f, 0.10f, 0.80f, // 35 - Hot Pink
    0.80f, 0.60f, 0.50f  // 36 - Peach
};


void GenerateBlocks(std::vector<Block>& Blocks)
{
    int width   = 350;
    int height  = 350;
    int depth   = 350;

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < depth; ++k) {
                Blocks.emplace_back(
                    glm::vec3(i * 2.0f, j * 2.0f, k * 2.0f),
                    glm::mat4(1.0f)
                );
            }
        }
    }
}


int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }


    // Create game window
    Window gameWindow(1600, 900, "Test Window");


    // Check which GPU OpenGL is using
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);

    std::cout << "OpenGL vendor: "
              << (vendor
                  ? reinterpret_cast<const char*>(vendor)
                  : "Unknown")
              << '\n';

    std::cout << "OpenGL renderer: "
              << (renderer
                  ? reinterpret_cast<const char*>(renderer)
                  : "Unknown")
              << '\n';


    Camera buddy;
    Shader shader;


    std::vector<Block> Blocks;
    Blocks.reserve(350 * 350 * 350);

    GenerateBlocks(Blocks);


    // Creating model matrix for each block in Blocks
    for (Block& block : Blocks) {
        block.model = glm::translate(
            glm::mat4(1.0f),
            block.position
        );
    }


    // Instanced Rendering:
    // Upload block positions to a GPU buffer.
    std::vector<glm::vec3> positions;

    for (const Block& block : Blocks) {
        positions.push_back(block.position);
    }


    // Creating Shader Program
    const std::string vertexShaderSourceString =
        ReadTextFile("src/graphics/shaders/vertexShader.glsl");

    const std::string fragmentShaderSourceString =
        ReadTextFile("src/graphics/shaders/fragmentShader.glsl");

    const char* vertexShaderSource =
        vertexShaderSourceString.c_str();

    const char* fragmentShaderSource =
        fragmentShaderSourceString.c_str();


    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;

    vertexShader = shader.BuildShader(
        vertexShader,
        vertexShaderSource,
        'v'
    );

    fragmentShader = shader.BuildShader(
        fragmentShader,
        fragmentShaderSource,
        'f'
    );

    unsigned int shaderProgram =
        shader.CreateShaderProgram(vertexShader, fragmentShader);


    // Generate and bind the VAO
    VertexArray vao;
    vao.bind();


    // POSITION
    VertexBuffer vbo;
    vbo.bind();

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0
    );

    glEnableVertexAttribArray(0);


    // COLOUR
    VertexBuffer cbo;
    cbo.bind();

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(colours),
        colours,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0
    );

    glEnableVertexAttribArray(1);

    // ELEMENT BUFFER
    VertexBuffer ebo;
    ebo.bind(GL_ELEMENT_ARRAY_BUFFER);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // INSTANCE
    VertexBuffer ibo;
    ibo.bind();

    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size() * sizeof(glm::vec3),
        positions.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0
    );

    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);


    // CAMERA UNIFORMS
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");


    glUseProgram(shaderProgram);

    glUniformMatrix4fv(
        projLoc,
        1,
        GL_FALSE,
        glm::value_ptr(buddy.cameraProjection)
    );
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    vao.unbind();

    // enable stencil tes
    //glEnable(GL_STENCIL_TEST);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


    // --------------------------------------------------
    // FPS COUNTER INITIALIZATION
    // --------------------------------------------------

    auto lastTime = clock_type::now();
    auto lastFPSTime = lastTime;

    int frameCount = 0;
    int fps = 0;


    // --------------------------------------------------
    // MAIN RENDER LOOP
    // --------------------------------------------------

    while (!gameWindow.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);


        // Frame timing
        auto currentTime = clock_type::now();

        double deltaTime =
            std::chrono::duration<double>(
                currentTime - lastTime
            ).count();

        lastTime = currentTime;


        // Clear buffers at the start of each frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);


        // Activate shader program
        glUseProgram(shaderProgram);


        // Update camera position and view matrix
        buddy.cameraView =
            buddy.updateCameraPosition(
                gameWindow.getNativeWindow()
            );

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(buddy.cameraView)
        );


        // Activate VAO
        glBindVertexArray(vao.getRendererID());


        // Original non-instanced rendering approach:
        /*
        for (Block& block : Blocks) {
            glUniformMatrix4fv(
                modelLoc,
                1,
                GL_FALSE,
                glm::value_ptr(block.model)
            );

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */

/*
        // Instanced rendering
        glDrawArraysInstanced(
            GL_TRIANGLES,
            0,
            36,
            static_cast<GLsizei>(Blocks.size())
        );
*/

        glDrawElementsInstanced(
            GL_TRIANGLES,
            static_cast<GLsizei>(sizeof(indices) / sizeof(indices[0])),
            GL_UNSIGNED_INT,
            nullptr,
            static_cast<GLsizei>(Blocks.size())
        );



        // Double buffering
        glfwSwapBuffers(gameWindow.getNativeWindow());
        glfwPollEvents();


        // --------------------------------------------------
        // FPS COUNTER
        // --------------------------------------------------

        frameCount++;

        auto fpsCurrentTime = clock_type::now();

        double elapsed =
            std::chrono::duration<double>(
                fpsCurrentTime - lastFPSTime
            ).count();

        // Update the FPS counter approximately once per second
        if (elapsed >= 1.0)
        {
            fps = static_cast<int>(frameCount / elapsed);

            std::string title =
                "Voxel Engine | FPS: " + std::to_string(fps);

            glfwSetWindowTitle(
                gameWindow.getNativeWindow(),
                title.c_str()
            );

            // Reset the counter for the next interval
            frameCount = 0;
            lastFPSTime = fpsCurrentTime;
        }


        gameWindow.update();
    }


    return 0;
}