#include "Window.h"
#include <iostream>

#ifdef DEBUG
#define GLFW_ERROR_LOG
#endif // DEBUG

static void onGLFWError(int err, const char* desc)
{
    std::cerr << "GLFW Error : " << desc << std::endl;
}

Window::Window(int width, int height, const char* title)
{
    if (!glfwInit()) {
        std::cerr << "glfwInitialization Failed! " << std::endl;
        return;
    }

#ifdef GLFW_ERROR_LOG
    glfwSetErrorCallback(onGLFWError);
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window! " << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Glad Failed load OpenGL" << std::endl;
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        return;
    }

    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    SetUserDataPointer(nullptr);
}

bool Window::MouseBtnPressed(MouseBtn btn)
{
    if (btn == MouseBtn::LEFT)
        return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
    else
        return glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT);
}

bool Window::IsPressed(int key)
{
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

float Window::GetTime()
{
    return (float)glfwGetTime();
}

Vec2f Window::GetMousePos()
{
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    return Vec2f((float)xpos, (float)ypos);
}

void Window::Poll()
{
    glfwPollEvents();
}

void Window::Update()
{
    glfwSwapBuffers(m_Window);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}