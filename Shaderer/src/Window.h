#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MathVector.h"

enum class MouseBtn
{
    LEFT,
    RIGHT
};

class Window
{
public:
    Window(int width, int height, const char* title);

    bool MouseBtnPressed(MouseBtn btn);
    bool IsPressed(int key);
    float GetTime();
    Vec2f GetMousePos();
    inline void SetKeyCallback(GLFWkeyfun cb) { glfwSetKeyCallback(m_Window, cb); }
    inline void SetScrollCallback(GLFWscrollfun cb) { glfwSetScrollCallback(m_Window, cb); }
    inline void SetMousePosCallback(GLFWcursorposfun cb) { glfwSetCursorPosCallback(m_Window, cb); }
    inline void SetResizeCb(GLFWframebuffersizefun cb) { glfwSetFramebufferSizeCallback(m_Window, cb); }
    inline void SetUserDataPointer(void* data) { glfwSetWindowUserPointer(m_Window, data); }
    inline void SetSwapInterval(int i) { glfwSwapInterval(i); }
    inline bool Running() { return !glfwWindowShouldClose(m_Window); }
    void Poll();
    void Update();

    ~Window();

// private: Make it private
    GLFWwindow* m_Window = nullptr;
};
