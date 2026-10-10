#include "math/Camera.h"

Camera::Camera() {
    cameraPos = glm::vec3(2.0f,2.0f,2.0f);
    cameraTarget = glm::vec3(0.0f,0.0f,0.0f);
    cameraUp = glm::vec3(0.0f,1.0f,0.0f);

    cameraView = glm::lookAt(cameraPos,cameraTarget,cameraUp);

    fov = glm::radians(45.0f);
    aspectRatio = 1200.0f/800.0f;
    nearPlane = 0.1f; 
    farPlane = 100.0f;

    cameraProjection = glm::perspective(fov,aspectRatio,nearPlane,farPlane);


}

