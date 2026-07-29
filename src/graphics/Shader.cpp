#include "graphics/Shader.h"

const char* VertexShader::CreateShaderSource(char type) {
    if (type == 'v') {
        return R"(
            #version 460 core

            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;

            out vec3 vertexColor;

            void main()
            {
                vertexColor = aColor;
                gl_Position = vec4(aPos, 1.0);
            }
        )";
    }
    else if (type == 'f') {
        return R"(
            #version 460 core

            in vec3 vertexColor;
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(vertexColor, 1.0);
            }
        )";
    }
    else {
        std::cout << "invalid shader type use v or f" << std::endl;
        return nullptr;
    }
}
unsigned int VertexShader::BuildShader(unsigned int shader, const char* shaderSource, char type) {

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
            
            glShaderSource(shader, 1, &shaderSource, nullptr);
            glCompileShader(shader);

            int success;
            char infoLog[512];

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success) {
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);

                std::cerr << "Shader Compilation Failed :\n" << infoLog << std::endl;
            }

            return shader;
}

unsigned int VertexShader::CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
            unsigned int shaderProgram = glCreateProgram();
            int success;
            char infoLog[512];

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
            
            return shaderProgram;
}