#pragma once
#include "Window.h"
#include "ImGui/ImGuiLayer.h"

namespace Flameberry {
    class Application
    {
    public:
        Application();
        ~Application();

        void Render();
        Window& GetMainWindow() { return m_Window; }
        static Application* GetApp() { return s_App; }

        void Run();
    private:
        Window m_Window;
        ImGuiLayer m_ImGuiLayer;

        uint32_t m_ViewportWidth, m_ViewportHeight;
        uint32_t* m_RenderImageData;
        uint32_t m_RenderImageTextureId;
    private:
        static Application* s_App;
    };
}