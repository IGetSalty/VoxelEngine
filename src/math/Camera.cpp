#include "math/Camera.h"
#include <cmath>

Camera::Camera() {
    cameraPos       = glm::vec3(4.0f, 4.0f, 4.0f);
    cameraTarget    = glm::vec3(50.0f,50.0f, 50.0f);
    cameraUp        = glm::vec3(0.0f, 1.0f, 0.0f);

    radius      = 500; //sqrt(12.0f);
    pitch       = 0.35f;
    yaw         = 0.0f;
    moveSpeed   = 0.025f;

    cameraView  = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    fov         = glm::radians(45.0f);
    aspectRatio = 1920.0f / 1080.0f;
    nearPlane   = 0.1f;
    farPlane    = 1000.0f;

    cameraProjection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::updateCameraPosition(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pitch += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pitch -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw += moveSpeed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        radius -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        radius += moveSpeed;

    pitch = glm::clamp(pitch, -1.4f, 1.4f);

    if (radius < 2.0f) radius = 2.0f;

    cameraPos.x = cameraTarget.x+radius*std::cos(yaw)* std::cos(pitch);
    cameraPos.y = cameraTarget.y+radius*std::sin(pitch);
    cameraPos.z = cameraTarget.z+radius*std::sin(yaw)* std::cos(pitch);

    cameraView = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    return cameraView;
}
