#include "Window.h"
#include <glad/glad.h>
#include <iostream>

namespace Flameberry {
    std::shared_ptr<Window> Window::Create(uint32_t width, uint32_t height, const char* title, bool vsync)
    {
        return std::make_shared<Window>(width, height, title, vsync);
    }

    Window::Window(uint32_t width, uint32_t height, const char* title, bool vsync)
        : m_Width(width), m_Height(height), m_Title(title), m_Vsync(vsync)
    {
        if (!glfwInit())
            std::cout << "Failed to Initialize GLFW!" << std::endl;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (!m_Window)
            std::cout << "Window is null!" << std::endl;

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        int actualWidth, actualHeight;
        glfwGetFramebufferSize(m_Window, &actualWidth, &actualHeight);
        glViewport(0, 0, actualWidth, actualHeight);
    }

    uint32_t Window::GetWidth()
    {
        return m_Width;
    }

    uint32_t Window::GetHeight()
    {
        return m_Height;
    }

    void Window::UpdateDimensions()
    {
        glfwGetWindowSize(m_Window, (int*)&m_Width, (int*)&m_Height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate() const
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    bool Window::IsRunning() const
    {
        return !(glfwWindowShouldClose(m_Window));
    }
}
