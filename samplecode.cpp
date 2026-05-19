#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

// ==================================================
// Shader Sources
// ==================================================

const char* vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 460 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.2, 0.8, 0.3, 1.0);
}
)";

// ==================================================
// Smart Pointer Deleter
// ==================================================

struct GLFWWindowDeleter
{
    void operator()(GLFWwindow* window) const
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }
    }
};

// ==================================================
// Input
// ==================================================

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// ==================================================
// Main
// ==================================================

int main()
{
    // ==================================================
    // Initialize GLFW
    // ==================================================

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ==================================================
    // Create Window
    // ==================================================

    std::unique_ptr<GLFWwindow, GLFWWindowDeleter> window(
        glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr)
    );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window.get());

    // ==================================================
    // Initialize GLAD
    // ==================================================

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    // ==================================================
    // Build Vertex Shader
    // ==================================================

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

        std::cerr << "Vertex Shader Compilation Failed:\n"
                  << infoLog << '\n';
    }

    // ==================================================
    // Build Fragment Shader
    // ==================================================

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

        std::cerr << "Fragment Shader Compilation Failed:\n"
                  << infoLog << '\n';
    }

    // ==================================================
    // Create Shader Program
    // ==================================================

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

        std::cerr << "Shader Program Linking Failed:\n"
                  << infoLog << '\n';
    }

    // Shaders are no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ==================================================
    // Triangle Vertex Data
    // ==================================================

    float vertices[] =
    {
         0.0f,  0.5f, 0.0f,  // Top
        -0.5f, -0.5f, 0.0f, // Bottom Left
         0.5f, -0.5f, 0.0f  // Bottom Right
    };

    // ==================================================
    // VBO (Vertex Buffer Object)
    // ==================================================
    // Stores raw vertex data on the GPU
    // ==================================================

    unsigned int VBO;

    glGenBuffers(1, &VBO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertex data into GPU memory
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Optional unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ==================================================
    // VAO (Vertex Array Object)
    // ==================================================
    // Stores HOW OpenGL should interpret the VBO
    // ==================================================

    unsigned int VAO;

    glGenVertexArrays(1, &VAO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind VBO again so VAO can remember it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Describe vertex layout
    glVertexAttribPointer(
        0,                  // Attribute index
        3,                  // 3 floats per vertex
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),  // Stride
        (void*)0            // Offset
    );

    // Enable attribute
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);

    // Optional unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ==================================================
    // Render Loop
    // ==================================================

    while (!glfwWindowShouldClose(window.get()))
    {
        processInput(window.get());

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Bind VAO
        glBindVertexArray(VAO);

        // Draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        glfwSwapBuffers(window.get());

        // Poll events
        glfwPollEvents();
    }

    // ==================================================
    // Cleanup
    // ==================================================

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}