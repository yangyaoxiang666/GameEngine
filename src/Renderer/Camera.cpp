#include "Renderer/Camera.h"

// OpenGL / GLFW 头文件顺序必须：先 glad 再 glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(GLFWwindow* window, float dt)
{
    float velocity = speed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * velocity;

    glm::vec3 right = glm::normalize(glm::cross(front, up));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // 限制俯仰角，避免“抬头抬到翻过去”
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    // 由 yaw / pitch 算出新的 front 向量
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
    // up 保持世界上方向即可（简单版本），以后需要可以算 right & up
}

