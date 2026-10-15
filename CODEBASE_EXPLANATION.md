# VoxelEngine Codebase Explanation

This document explains the project code that is not excluded by your rule: it intentionally omits the `.gitignore`, CMake files, and documentation files such as `.md`, `.txt`, and `.odt`.

---

## 1) Root script: `run.sh`

File: `run.sh`

```bash
#!/usr/bin/env bash
set -Eeuo pipefail

cd "$(dirname "$0")"

echo "=== Cleaning build ==="
rm -rf build

sleep 1

echo "=== Configuring CMake ==="
cmake -B build

echo "=== Building VoxelEngine ==="
cmake --build build

echo "=== Launching VoxelEngine ==="
exec ./build/VoxelEngine
```

What each line does:

- `#!/usr/bin/env bash`: tells Unix-like systems to run the script with Bash.
- `set -Eeuo pipefail`: makes the script fail fast on errors and when an unset variable is used.
- `cd "$(dirname "$0")"`: moves into the directory containing the script so it always runs from the repository root.
- `echo "=== Cleaning build ==="`: prints a status message.
- `rm -rf build`: deletes the previous build directory so the project recompiles cleanly.
- `sleep 1`: pauses the script for one second to allow filesystem cleanup to finish.
- `cmake -B build`: configures the project into the `build/` directory.
- `cmake --build build`: compiles the project using the generated build files.
- `exec ./build/VoxelEngine`: replaces the shell process with the built executable so the app starts immediately.

This is a standard one-command build-and-launch helper for the project.

---

## 2) Entry point: `src/main.cpp`

This file is the program entry point. It wires up GLFW, creates the window, builds the shader program, loads vertices, and starts the render loop.

```cpp
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
```

These includes pull in the OpenGL loader, GLFW windowing API, GLM math library, custom project classes, and file-reading helper.

```cpp
//#include <memory>
#include <iostream>
#include <string>
//#include <fstream>
//#include <sstream>
#include <vector>
```

These are basic standard library headers; the commented-out lines suggest the author tried different approaches earlier and commented them out.

```cpp
//this is a test for committ.
```

A stray comment left by the developer during testing.

```cpp
static const GLfloat vertices[] = {
    ...
};
```

This array stores a cube’s vertex positions. It is a hard-coded 3D cube mesh made of triangles. The numbers are XYZ coordinate triplets for the cube’s corners.

```cpp
static const GLfloat colours[] = {
    ...
};
```

This array stores per-vertex colors. It matches the `vertices` array so each point has a color. This is used to tint the cube faces to make the demo visually interesting.

```cpp
void GenerateBlocks(std::vector<Block>& Blocks) {
    int width = 100;
    int height = 100;
    int depth = 100;
```

This function creates a 100x100x100 grid of `Block` objects. In practice, that is a huge amount of data and is mostly a prototype/placeholder for a voxel world.

```cpp
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
```

This loops through the volume and creates a block at every grid coordinate. Each block stores a position and a model matrix.

```cpp
int main() {
```

The application starts here.

```cpp
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }
```

This initializes GLFW, the windowing library. If it fails, the program prints an error and exits.

```cpp
    Window gameWindow(1920,1080,"Test Window");
    Camera buddy; 
    Shader shader; 
```

Creates the window, a camera, and a shader wrapper.

```cpp
    std::vector<Block> Blocks;
    Blocks.reserve(100*100*100);
    GenerateBlocks(Blocks);
```

Allocates a vector for blocks and pre-allocates memory. Then populates the vector with generated block positions.

```cpp
    for (Block& block : Blocks) {
        block.model = glm::translate(
                            glm::mat4(1.0f), 
                            block.position
                        );
    }
```

For each block, this builds a model matrix that translates the mesh into its world position.

```cpp
    std::vector<glm::vec3> positions;
    for (const Block& block : Blocks) {
        positions.push_back(block.position);
    }
```

This creates a second list containing only the world positions of the blocks. That is later uploaded to the GPU as instance data.

```cpp
    const std::string vertexShaderSourceString = ReadTextFile("src/graphics/shaders/vertexShader.glsl");
    const std::string fragmentShaderSourceString = ReadTextFile("src/graphics/shaders/fragmentShader.glsl");
```

Reads the vertex and fragment shader source code from files on disk.

```cpp
    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
```

Converts the shader source strings into raw C strings so OpenGL can compile them.

```cpp
    unsigned int vertexShader = 0, fragmentShader = 0;
    vertexShader = shader.BuildShader(vertexShader, vertexShaderSource, 'v');
    fragmentShader = shader.BuildShader(fragmentShader, fragmentShaderSource, 'f');
```

Creates the individual shader objects and compiles them.

```cpp
    unsigned int shaderProgram = shader.CreateShaderProgram(vertexShader, fragmentShader);
```

Links the compiled vertex and fragment shaders into one OpenGL shader program.

```cpp
    VertexArray vao;
    vao.bind(); 
```

Creates and binds the vertex array object (VAO), which stores the pipeline state for the geometry.

```cpp
    VertexBuffer vbo;
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
```

This uploads the cube vertex positions to a GPU buffer and tells OpenGL that attribute location `0` is a 3D float position.

```cpp
    VertexBuffer cbo;
    cbo.bind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(colours),colours,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
```

This does the same thing for colors, using attribute location `1`.

```cpp
    VertexBuffer ibo;
    ibo.bind();
    glBufferData(GL_ARRAY_BUFFER,positions.size()*sizeof(glm::vec3), positions.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2,1);
```

This creates an instanced buffer that holds per-block positions. `glVertexAttribDivisor(2,1)` tells OpenGL to advance to a new instance for each block, which is necessary for instanced rendering.

```cpp
    GLint modelLoc  = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc   = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc   = glGetUniformLocation(shaderProgram, "projection");
```

Looks up the shader uniforms for the model, view, and projection matrices.

```cpp
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(buddy.cameraProjection)); 
```

Activates the shader program and uploads the camera projection matrix once.

```cpp
    vao.unbind(); 
```

Unbinds the VAO. It is still valid and will be bound again in the render loop.

```cpp
    glEnable(GL_DEPTH_TEST);
```

Enables depth testing so objects behind each other are drawn correctly.

```cpp
    while (!gameWindow.shouldClose())
    {
```

Starts the main application loop. It continues until the window is closed.

```cpp
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
```

Sets the clear color to dark black with transparency.

```cpp
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

Clears the color and depth buffers before drawing each frame.

```cpp
        glUseProgram(shaderProgram);
```

Makes sure the shader program is in use before drawing.

```cpp
        buddy.cameraView = buddy.updateCameraPosition(gameWindow.getNativeWindow());
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(buddy.cameraView));
```

Updates the camera based on keyboard input and sends the result to the GPU.

```cpp
        glBindVertexArray(vao.getRendererID());
```

Binds the vertex array object so the draw call uses the uploaded geometry state.

```cpp
        glDrawArraysInstanced(GL_TRIANGLES,0,36,static_cast<GLsizei>(Blocks.size()));
```

Draws multiple copies of the one cube mesh, one per block, using instancing. This is the core optimization that allows many blocks to be drawn efficiently.

```cpp
        glfwSwapBuffers(gameWindow.getNativeWindow());
        glfwPollEvents();

        gameWindow.update();
    }
```

Swaps the back and front buffers to display the frame, processes OS events, and updates the window wrapper state.

The file is a working prototype for a voxel-rendering demo using OpenGL instancing.

---

## 3) Window wrapper: `src/core/Window.cpp`

This file implements the custom `Window` class defined in `include/core/Window.h`.

```cpp
#include "core/Window.h"
```

Includes the class header.

```cpp
Window::Window(int width, int height, const std::string& title)
    : m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr) {
        init();
}
```

Constructor: stores the window dimensions and title, then calls `init()`.

```cpp
void Window::update() {
    glfwPollEvents();
}
```

This polls for window/system events every update tick.

```cpp
bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_Window);
}
```

Checks whether the user has requested the window close.

```cpp
GLFWwindow* Window::getNativeWindow() const {
    return m_Window;
}
```

Returns the raw GLFW window pointer needed for OpenGL calls and camera input.

```cpp
int Window::getWidth() const {
    return m_Width;
}
```

Returns the current width.

```cpp
int Window::getHeight() const {
    return m_Height;
}
```

Returns the current height.

```cpp
void Window::init() {
```

Initializes the GLFW window and OpenGL context.

```cpp
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
```

Configures OpenGL 4.6 core profile, which is the modern profile the engine expects.

```cpp
    m_Window = glfwCreateWindow(m_Width,m_Height,m_Title.c_str(),nullptr,nullptr);
```

Creates the actual window with the configured size and title.

```cpp
    if (!m_Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
```

If creation fails, it prints an error and terminates the app.

```cpp
    glfwMakeContextCurrent(m_Window);
```

Makes the created window the active OpenGL context.

```cpp
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to Initialise GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
```

Loads OpenGL function pointers through GLAD.

```cpp
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            win->m_Width = width;
            win->m_Height = height;
            glViewport(0, 0, width, height);
    });
```

This registers a resize callback. When the user resizes the window, the callback updates the stored dimensions and resets the viewport to match the new framebuffer size.

---

## 4) Camera logic: `src/math/Camera.cpp`

This file implements the camera controller.

```cpp
#include "math/Camera.h"
#include <cmath>
```

Includes the camera declaration and math functions for `cos()` and `sin()`.

```cpp
Camera::Camera() {
    cameraPos       = glm::vec3(4.0f, 4.0f, 4.0f);
    cameraTarget    = glm::vec3(50.0f,50.0f, 50.0f);
    cameraUp        = glm::vec3(0.0f, 1.0f, 0.0f);
```

Sets a starting camera position, target point, and upward vector.

```cpp
    radius      = 500; //sqrt(12.0f);
    pitch       = 0.35f;
    yaw         = 0.0f;
    moveSpeed   = 0.025f;
```

Defines camera orbit parameters: distance from target, vertical angle, horizontal angle, and speed.

```cpp
    cameraView  = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

Builds the initial view matrix from the camera position and target.

```cpp
    fov         = glm::radians(45.0f);
    aspectRatio = 1920.0f / 1080.0f;
    nearPlane   = 0.1f;
    farPlane    = 1000.0f;
```

Sets the perspective projection values.

```cpp
    cameraProjection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
```

Builds the projection matrix.

```cpp
glm::mat4 Camera::updateCameraPosition(GLFWwindow *window) {
```

This is called once per frame to update the camera from keyboard input.

```cpp
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pitch += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pitch -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw += moveSpeed;
```

WASD rotates the camera orbit around the target.

```cpp
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        radius -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        radius += moveSpeed;
```

Q/E move the camera closer to or farther from the target.

```cpp
    pitch = glm::clamp(pitch, -1.4f, 1.4f);
```

Limits vertical camera motion so the view does not flip over.

```cpp
    if (radius < 2.0f) radius = 2.0f;
```

Prevents the camera from getting too close to the target.

```cpp
    cameraPos.x = cameraTarget.x+radius*std::cos(yaw)* std::cos(pitch);
    cameraPos.y = cameraTarget.y+radius*std::sin(pitch);
    cameraPos.z = cameraTarget.z+radius*std::sin(yaw)* std::cos(pitch);
```

Computes a spherical orbit around the target point using yaw and pitch.

```cpp
    cameraView = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

Builds a fresh view matrix from the new camera position.

```cpp
    return cameraView;
}
```

Returns the updated view matrix for use in the shader.

---

## 5) Shader compilation: `src/graphics/Shader.cpp`

This file implements the wrapper around OpenGL shader compilation and linking.

```cpp
#include "graphics/Shader.h"
```

Includes the shader class definition.

```cpp
unsigned int Shader::BuildShader(unsigned int shader, const char* shaderSource, char type) {
```

Creates and compiles either a vertex or fragment shader.

```cpp
            if (type == 'v') {
                shader = glCreateShader(GL_VERTEX_SHADER);
            }
            else if (type == 'f') {
                shader = glCreateShader(GL_FRAGMENT_SHADER);
            }
            else {
                std::cout << "invalid shader type use v or f" << std::endl;
                return 0;
            }
```

Checks the type value and creates the appropriate OpenGL shader object. If the type is invalid, it logs an error and exits early.

```cpp
            glShaderSource(shader, 1, &shaderSource, nullptr);
            glCompileShader(shader);
```

Uploads the shader source text and asks OpenGL to compile it.

```cpp
            int success;
            char infoLog[512];
```

Declares variables to check whether compilation succeeded and to store compiler messages.

```cpp
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
```

Queries the shader compile status.

```cpp
            if (!success) {
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);

                std::cerr << "Shader Compilation Failed :\n" << infoLog << std::endl;
            }
```

If compilation fails, it prints the OpenGL shader compiler error log.

```cpp
            return shader;
}
```

Returns the compiled shader handle.

```cpp
unsigned int Shader::CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
```

Creates an OpenGL program from two already-compiled shaders.

```cpp
            unsigned int shaderProgram = glCreateProgram();
            int success;
            char infoLog[512];
```

Allocates a shader program and variables for linking errors.

```cpp
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
```

Attaches the compiled GPU shader objects to the program.

```cpp
            glLinkProgram(shaderProgram);
```

Links the shaders into a working executable program on the GPU.

```cpp
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
```

Checks whether the program linked successfully.

```cpp
            if (!success)
            {
                glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

                std::cerr << "Shader Program Linking Failed:\n"
                  << infoLog << '\n';
            }
```

Logs linking errors if the program cannot be built.

```cpp
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
```

Deletes the intermediate individual shaders because they are no longer needed after linking.

```cpp
            return shaderProgram;
}
```

Returns the final linked shader program for use in rendering.

---

## 6) Vertex array wrapper: `src/graphics/VertexArray.cpp`

This file wrappers OpenGL’s `glGenVertexArrays` and related functions.

```cpp
#include "graphics/VertexArray.h"

#include <glad/glad.h>
```

Includes the class header and OpenGL loader.

```cpp
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}
```

Creates one OpenGL vertex array object and stores its ID in `m_RendererID`.

```cpp
VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}
```

Destroys the VAO when the object goes out of scope.

```cpp
void VertexArray::bind() const
{
    glBindVertexArray(m_RendererID);
}
```

Binds the VAO so subsequent vertex attribute calls apply to it.

```cpp
void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
```

Unbinds the VAO by binding the default null VAO.

```cpp
unsigned int VertexArray::getRendererID() const {
    return m_RendererID;
}
```

Returns the VAO ID for use elsewhere in the engine.

---

## 7) Vertex buffer wrapper: `src/graphics/VertexBuffer.cpp`

This file wraps the OpenGL buffer object used for vertex attribute data.

```cpp
#include "graphics/VertexBuffer.h"

#include <glad/glad.h>
```

Includes the class header and GLAD.

```cpp
VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_RendererID);

}
```

Creates a buffer ID.

```cpp
VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}
```

Deletes the GPU buffer when the object is destroyed.

```cpp
void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}
```

Binds the buffer as the active array buffer.

```cpp
void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
```

Unbinds the array buffer.

```cpp
unsigned int VertexBuffer::getRendererID() const {
    return m_RendererID;
}
```

Returns the handle to the GPU buffer.

---

## 8) Shader source: `src/graphics/shaders/vertexShader.glsl`

```glsl
#version 460 core
uniform mat4 view,projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aInstancePosition;

out vec3 vertexColor;

void main()
{
    vec3 worldPosition = aPos*aInstancePosition;
    vertexColor = aColor;
    gl_Position = projection * view * vec4(worldPosition, 1.0);
}
```

What this does:

- `#version 460 core`: uses OpenGL 4.6 core GLSL.
- `uniform mat4 view,projection;`: declares camera matrices from C++.
- `layout (location = 0) in vec3 aPos;`: vertex position attribute.
- `layout (location = 1) in vec3 aColor;`: vertex color attribute.
- `layout (location = 2) in vec3 aInstancePosition;`: transforms the cube by each block’s instance position.
- `out vec3 vertexColor;`: passes the color to the fragment shader.
- `vec3 worldPosition = aPos*aInstancePosition;`: multiplies the local vertex position by the instance/world offset.
- `gl_Position = projection * view * vec4(worldPosition, 1.0);`: writes the final clip-space position.

This shader is responsible for turning each cube’s local vertices into the final screen position.

---

## 9) Fragment shader: `src/graphics/shaders/fragmentShader.glsl`

```glsl
#version 460 core

in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
```

What this does:

- `#version 460 core`: sets GLSL version.
- `in vec3 vertexColor;`: receives the color from the vertex shader.
- `out vec4 FragColor;`: writes the final pixel color.
- `FragColor = vec4(vertexColor, 1.0);`: sets the pixel to the vertex color with full alpha.

This is the final stage that colors each rendered pixel.

---

## 10) Block implementation: `src/world/Block.cpp`

```cpp
#include "Block.h"
```

This file is currently just an include for the `Block` declaration. It does not add logic yet. The real structure is in the header file.

---

## 11) OpenGL loader: `src/glad.c`

This file is generated by the GLAD tool. It is not hand-written project logic; it is the platform-specific loader that binds OpenGL function pointers to the runtime environment.

The file contains code for:

- loading OpenGL entry points;
- handling Windows, Linux, and macOS differences;
- retrieving function pointers through the OS loader;
- exposing the OpenGL API to the rest of the program.

Because it is generated, the code is mechanically produced and not meant to be manually edited.

---

## 12) Header: `include/core/Window.h`

```cpp
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

class Window {
```

- `#pragma once`: prevents multiple inclusion of the header.
- `#include <glad/glad.h>` and `#include <GLFW/glfw3.h>`: bring in OpenGL and windowing APIs.
- `#include <memory>` and `#include <iostream>`: include memory and standard I/O support.
- `class Window`: declares the custom wrapper class around GLFW window functionality.

```cpp
    public:
        Window(int width, int height, const std::string& title);
        
        void update();

        bool shouldClose() const;

        GLFWwindow* getNativeWindow() const;

        int getHeight() const;
        int getWidth() const;
```

Public interface:

- constructor sets width, height, and title;
- `update()` polls events;
- `shouldClose()` checks if the window is closing;
- `getNativeWindow()` accesses the underlying GLFW handle;
- `getHeight()` and `getWidth()` expose the current dimensions.

```cpp
    private:
        int m_Width;
        int m_Height;
        std::string m_Title;
        GLFWwindow* m_Window;

        void init();

};
```

Private state:

- `m_Width` and `m_Height`: current size;
- `m_Title`: window title;
- `m_Window`: pointer to the actual GLFW window object;
- `init()`: initializes the OpenGL context and window settings.

---

## 13) Header: `include/math/Camera.h`

```cpp
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```

Imports OpenGL, GLFW, and GLM support for matrices and vectors.

```cpp
struct Camera
{
    public:
        Camera();
        //~Camera();
        unsigned int GetRendererID();
```

The struct defines a camera object with a constructor, a commented-out destructor, and a method to retrieve an ID-like render handle.

```cpp
        //GLint viewLoc();
        //GLint projLoc();
        
        GLint getRendererID();
        glm::mat4 cameraView;
        glm::mat4 cameraProjection;
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget, cameraUp;
```

These are the actual camera state values:

- `cameraView`: matrix describing where the camera looks;
- `cameraProjection`: perspective projection;
- `cameraPos`: camera location;
- `cameraTarget`: point the camera looks at;
- `cameraUp`: axis used for orientation;
- `getRendererID()`: placeholder-style accessor.

```cpp
        float radius;
        float pitch;
        float yaw;
        float moveSpeed;
```

These fields define the orbital camera movement style.

```cpp
        glm::mat4 updateCameraPosition(GLFWwindow *window);
    
    private:
        GLint m_rendererID();
        
        float fov, aspectRatio, nearPlane, farPlane;
```

- `updateCameraPosition` updates the camera every frame using keyboard input.
- `m_rendererID` is a private placeholder-style ID accessor.
- `fov`, `aspectRatio`, `nearPlane`, and `farPlane` define the projection matrix used by the camera.

This header is a lightweight camera definition for the scene camera.

---

## 14) Header: `include/graphics/Shader.h`

```cpp
#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
    public:

        unsigned int BuildShader(unsigned int shader, const char* shaderSource, char type);

        unsigned int CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);     

};
```

This is a very small wrapper for shader creation.

- `BuildShader(...)`: compiles a vertex or fragment shader from source text.
- `CreateShaderProgram(...)`: links multiple shader stages into one GPU program.
- The class is intentionally minimal and uses OpenGL API calls from the implementation file.

---

## 15) Header: `include/graphics/VertexArray.h`

```cpp
#pragma once
//test

class VertexArray
{
public:

    VertexArray();

    ~VertexArray();

    void bind() const;
    void unbind() const;

    unsigned int getRendererID() const;

private:

    unsigned int m_RendererID;
};
```

This header declares the class that keeps track of an OpenGL vertex array object.

- `VertexArray()` creates the VAO.
- `~VertexArray()` destroys it.
- `bind()` and `unbind()` bind or unbind it.
- `getRendererID()` returns the underlying GPU object ID.
- `m_RendererID` stores the generated OpenGL handle.

The `//test` line is a leftover comment.

---

## 16) Header: `include/graphics/VertexBuffer.h`

```cpp
#pragma once

class VertexBuffer
{
public:

    VertexBuffer();

    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getRendererID() const;

private:

    unsigned int m_RendererID;
};
```

This object wraps a generic OpenGL buffer that can hold vertex data. It exposes the same pattern as `VertexArray`:

- constructor generates a GPU buffer;
- destructor deletes it;
- `bind()` makes it active;
- `unbind()` clears the active binding;
- `getRendererID()` returns the buffer ID.

---

## 17) Helper: `include/util/FileHandling.h`

```cpp
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadTextFile(const std::string& filePath) {
    std::string fileContents;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    fileContents = buffer.str();

    file.close();

    return fileContents;
}
```

This helper loads a text file into a `std::string`.

- `std::ifstream file(filePath)`: opens the file.
- `if (!file.is_open())`: checks whether the file existed and could be opened.
- `std::stringstream buffer; buffer << file.rdbuf();`: reads the full file contents into a stream.
- `fileContents = buffer.str();`: converts the stream to a string.
- `return fileContents;`: sends the content back to the caller.

This is used to read the shader source files before compiling them.

---

## 18) Header: `include/world/Block.h`

```cpp
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Block {
    glm::vec3 position;
    glm::mat4 model;

};
```

This small struct represents one block in the world.

- `position`: the block’s world-space location.
- `model`: a transformation matrix that tells OpenGL where to place the block in space.

This is the core data object used by the instancing test in `src/main.cpp`.

---

## 19) Generated/OpenGL headers

### `include/glad/glad.h`

This is the generated OpenGL function loader header produced by GLAD. It contains thousands of OpenGL declarations, extension macros, and type definitions used to access OpenGL functions in a portable way.

It is not project-specific application logic; it is a vendor-generated compatibility layer for modern OpenGL.

### `include/KHR/khrplatform.h`

This is the Khronos platform header used by OpenGL-related APIs. It defines standard platform types and macros such as calling conventions and integer typedefs. It is also a generated/industry-standard compatibility header, not custom project code.

---

## 20) Empty placeholder files

These files currently exist but do not contain logic yet:

- `include/core/Application.h`
- `include/core/Input.h`
- `include/graphics/IndexBuffer.h`
- `include/graphics/Renderer.h`
- `include/graphics/Texture.h`
- `include/util/Timer.h`
- `include/world/Chunk.h`
- `include/world/World.h`

Their empty state means the project is still in an early prototype stage. They appear to be planned engine features that have not been implemented yet.

---

## 21) Overall architecture summary

The project is a small OpenGL prototype built around a few ideas:

- a GLFW window wrapper;
- a camera controller for orbiting the scene;
- shader compilation helpers;
- VBO/VAO wrappers;
- a simple block data structure;
- a fullscreen test render using instanced draw calls.

The engine is still highly experimental. Several headers are scaffolding, the world generation is hard-coded, and many data structures are placeholders rather than a full gameplay engine.

This codebase is best thought of as a learning/demo project for OpenGL rendering and basic voxel/world prototype work.
