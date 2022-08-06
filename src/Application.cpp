#include "Application.h"

namespace Flameberry {
    Application* Application::s_App;
    Application::Application()
    {
        s_App = this;
        EditorLayer::OnAttach();
    }

    Application::~Application()
    {
        EditorLayer::OnDetach();
        glfwTerminate();
    }

    void Application::Run()
    {
        while (m_Window.IsRunning())
        {
            EditorLayer::OnImGuiRender();
            m_Window.OnUpdate();
        }
    }
}