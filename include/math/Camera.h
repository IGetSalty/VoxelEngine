#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Camera
{
    public:
        Camera();
        //~Camera();
        unsigned int GetRendererID();
        
        //GLint viewLoc();
        //GLint projLoc();
        

        GLint getRendererID();
        glm::mat4 cameraView;
        glm::mat4 cameraProjection;

    
    private:
        GLint m_rendererID();
        glm::vec3 cameraPos, cameraTarget, cameraUp;
        float fov, aspectRatio, nearPlane, farPlane;



};