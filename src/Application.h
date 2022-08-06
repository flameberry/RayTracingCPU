#pragma once
#include "Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer.h"

namespace Flameberry {
    class Application
    {
    public:
        Application();
        ~Application();

        Window& GetMainWindow() { return m_Window; }
        static Application& GetApp() { return *s_App; }

        void Run();
    private:
        Window m_Window;
        Renderer m_CoreRenderer;

        ImGuiLayer m_ImGuiLayer;

        uint32_t m_ViewportWidth, m_ViewportHeight;
    private:
        static Application* s_App;
    };
}