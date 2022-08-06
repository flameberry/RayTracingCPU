#pragma once
#include <GLFW/glfw3.h>
#include <memory>

namespace Flameberry {
    class Window
    {
    public:
        Window(uint32_t width = 1280, uint32_t height = 720, const char* title = "Renderer", bool vsync = true);
        ~Window();

        static std::shared_ptr<Window> Create(uint32_t width = 1280, uint32_t height = 720, const char* title = "Renderer", bool vsync = true);
        void OnUpdate() const;
        inline GLFWwindow* GetGLFWwindow() const { return m_Window; }

        void UpdateDimensions();

        uint32_t GetWidth();
        uint32_t GetHeight();

        bool IsRunning() const;
    private:
        uint32_t m_Width, m_Height;
        GLFWwindow* m_Window;
    private:
        const char* m_Title;
        bool m_Vsync;
    };
}
