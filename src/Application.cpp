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
        float last = 0.0f;
        while (m_Window.IsRunning())
        {
            float now = glfwGetTime();
            float delta = now - last;
            last = now;

            m_EditorLayer.OnUpdate(delta);

            m_EditorLayer.OnImGuiRender();
            m_Window.OnUpdate();
        }
    }
}