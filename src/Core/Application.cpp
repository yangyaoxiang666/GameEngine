#include "Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

// ====== 鼠标控制相关的静态状态 ======
static bool  s_FirstMouse = true;
static float s_LastX      = 400.0f;
static float s_LastY      = 300.0f;

// 鼠标回调（和你原来的一样，只是放到这里）
static void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (s_FirstMouse)
    {
        s_LastX = (float)xpos;
        s_LastY = (float)ypos;
        s_FirstMouse = false;
    }

    float xoffset = (float)xpos - s_LastX;
    float yoffset = s_LastY - (float)ypos;

    s_LastX = (float)xpos;
    s_LastY = (float)ypos;

    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera)
    {
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

// ============= 构造 / 析构 =============
Application::Application()
{
}

Application::~Application()
{
    Shutdown();
}

// ============= 对外只暴露 Run =============
void Application::Run()
{
    Init();

    while (!m_Window->ShouldClose())
    {
        float currentTime = (float)glfwGetTime();
        float dt = currentTime - m_LastTime;
        m_LastTime = currentTime;

        ProcessInput(dt);
        Update(dt);
        Render();

        m_Window->SwapBuffers();
        m_Window->PollEvents();
    }
}

// ============= Init：把你 main 里的初始化搬过来 =============
void Application::Init()
{
    // Window（从 main 开头搬过来）
    m_Window = new Window(800, 600, "MyEngine - Textured Quad");
    m_GLFWwindow = m_Window->GetNativeWindow();

    glEnable(GL_DEPTH_TEST);

    // 顶点数据（原 main 里的 vertices）
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Shader & Texture（从 main 里搬过来）
    m_Shader = new Shader("D:/Dev/MyEngine/shaders/cube.vert",
                          "D:/Dev/MyEngine/shaders/cube.frag");

    m_Tex1 = new Texture("D:/Dev/MyEngine/assets/textures/container.png");
    m_Tex2 = new Texture("D:/Dev/MyEngine/assets/textures/awesomeface.png");

    m_Shader->Use();
    m_Shader->SetInt("u_Texture1", 0);
    m_Shader->SetInt("u_Texture2", 1);

    // 多个立方体的位置（从 main 里搬）
    m_Objects.clear();

    auto makeObj = [&](glm::vec3 pos, float speed) {
        GameObject obj;
        obj.transform.position = pos;
        obj.transform.scale    = glm::vec3(1.0f);  // 可以随便改
        obj.rotationSpeed      = speed;
        m_Objects.push_back(obj);
    };

    makeObj(glm::vec3( 0.0f,  0.0f,  0.0f),   1.0f);
    makeObj(glm::vec3( 2.0f,  5.0f, -15.0f),  1.5f);
    makeObj(glm::vec3(-1.5f, -2.2f, -2.5f),   0.8f);
    makeObj(glm::vec3(-3.8f, -2.0f, -12.3f),  1.2f);
    makeObj(glm::vec3( 2.4f, -0.4f, -3.5f),   2.0f);
    makeObj(glm::vec3(-1.7f,  3.0f, -7.5f),   0.6f);
    makeObj(glm::vec3( 1.3f, -2.0f, -2.5f),   1.8f);
    makeObj(glm::vec3( 1.5f,  2.0f, -2.5f),   1.1f);
    makeObj(glm::vec3( 1.5f,  0.2f, -1.5f),   2.2f);
    makeObj(glm::vec3(-1.3f,  1.0f, -1.5f),   0.9f);

    // 摄像机默认构造就够用

    // 鼠标相关（你 main 里的三行）
    glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(m_GLFWwindow, &m_Camera);
    glfwSetCursorPosCallback(m_GLFWwindow, MouseCallback);

    m_LastTime = (float)glfwGetTime();
}

// ============= 输入处理（WASD） =============
void Application::ProcessInput(float dt)
{
    m_Camera.ProcessKeyboard(m_GLFWwindow, dt);
}

// ============= 逻辑更新（先留空） =============
void Application::Update(float dt)
{
    for (auto& obj : m_Objects)
    {
        obj.transform.rotation.y += obj.rotationSpeed * dt;
    }
}

// ============= 渲染（把你 while 里的渲染部分搬过来） =============
void Application::Render()
{
    float currentTime = (float)glfwGetTime();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Use();

    glm::mat4 view = m_Camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    m_Shader->SetMat4("u_View",       view);
    m_Shader->SetMat4("u_Projection", projection);

    m_Tex1->Bind(0);
    glBindVertexArray(m_VAO);

    for (auto& obj : m_Objects)
    {
        glm::mat4 model = obj.transform.GetModelMatrix();
        m_Shader->SetMat4("u_Model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


// ============= Shutdown：释放资源 =============
void Application::Shutdown()
{
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);

    delete m_Shader; m_Shader = nullptr;
    delete m_Tex1;   m_Tex1 = nullptr;
    delete m_Tex2;   m_Tex2 = nullptr;

    delete m_Window; m_Window = nullptr;
}
