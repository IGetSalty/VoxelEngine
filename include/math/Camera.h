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
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget, cameraUp;

        float radius;
        float pitch;
        float yaw;
        float moveSpeed;

        glm::mat4 updateCameraPosition(GLFWwindow *window);
    
    private:
        GLint m_rendererID();
        
        float fov, aspectRatio, nearPlane, farPlane;



};