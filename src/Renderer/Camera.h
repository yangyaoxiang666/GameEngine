#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 只做前向声明，不真正包含头文件
struct GLFWwindow;

class Camera
{
public:
    glm::vec3 position{0.0f, 0.0f, 3.0f};   // 起始位置：往后 3 个单位
    glm::vec3 front{0.0f, 0.0f, -1.0f};     // 看向 -Z 方向
    glm::vec3 up{0.0f, 1.0f, 0.0f};         // 世界上方向

    float speed = 3.0f;                     // 移动速度（单位/秒）

    // 新增： 欧拉角
    float yaw = -90.0f; //初始朝向 -z
    float pitch = 0.0f;

    //鼠标灵敏度
    float mouseSensitivity = 0.1f;

    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(GLFWwindow* window, float dt);
    void ProcessMouseMovement(float xoffset, float yoffset);

private:
    void UpdateCameraVectors();
};
