#pragma once
#include "Window.h"
#include "ImGui/EditorLayer.h"

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
        EditorLayer m_EditorLayer;
    private:
        static Application* s_App;
    };
}