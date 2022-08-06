#include "Application.h"

namespace Flameberry {
    Application* Application::s_App;
    Application::Application()
    {
        s_App = this;
        m_EditorLayer.OnAttach();
    }

    Application::~Application()
    {
        m_EditorLayer.OnDetach();
        glfwTerminate();
    }

    void Application::Run()
    {
        while (m_Window.IsRunning())
        {
            m_EditorLayer.OnImGuiRender();
            m_Window.OnUpdate();
        }
    }
}