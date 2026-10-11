#include "math/Camera.h"
#include <cmath>

Camera::Camera() {
    cameraPos       = glm::vec3(2.0f, 2.0f, 2.0f);
    cameraTarget    = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraUp        = glm::vec3(0.0f, 1.0f, 0.0f);

    radius      = sqrt(12.0f);
    pitch       = 0.35f;
    yaw         = 0.0f;
    moveSpeed   = 0.025f;

    cameraView  = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    fov         = glm::radians(45.0f);
    aspectRatio = 1200.0f / 800.0f;
    nearPlane   = 0.1f;
    farPlane    = 100.0f;

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

    pitch = glm::clamp(pitch, -1.4f, 1.4f);

    cameraPos.x = cameraTarget.x+radius*std::cos(yaw)* std::cos(pitch);
    cameraPos.y = cameraTarget.y+radius*std::sin(pitch);
    cameraPos.z = cameraTarget.z+radius*std::sin(yaw)* std::cos(pitch);

    cameraView = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    return cameraView;
}
