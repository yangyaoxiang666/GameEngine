#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(int width, int height, const char* title)
    : m_Window(nullptr)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(-1);
    }

    // OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(m_Window);

    // 初始化 GLAD（一定要在这里）
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        std::exit(-1);
    }

    glViewport(0, 0, width, height);
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents() const {
    glfwPollEvents();
}

void Window::SwapBuffers() const {
    glfwSwapBuffers(m_Window);
}
