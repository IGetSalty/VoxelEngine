#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
    public:

        unsigned int BuildShader(unsigned int shader, const char* shaderSource, char type);

        unsigned int CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);     

};
