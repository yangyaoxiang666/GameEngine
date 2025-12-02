#pragma once

#include "Core/Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "GameObject/GameObject.h"

#include <glm/glm.hpp>
#include <vector>

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);   // 现在暂时空着，后面加 GameObject 再用
    void Render();
    void Shutdown();

private:
    Window*     m_Window      = nullptr;
    GLFWwindow* m_GLFWwindow  = nullptr;

    Shader*  m_Shader  = nullptr;
    Texture* m_Tex1    = nullptr;
    Texture* m_Tex2    = nullptr;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    Camera m_Camera;

    std::vector<GameObject> m_Objects;
    float m_LastTime = 0.0f;
};
