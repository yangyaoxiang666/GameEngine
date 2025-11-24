#pragma once

struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

    GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
    GLFWwindow* m_Window;
};
