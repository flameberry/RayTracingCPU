#pragma once
#include "../Renderer.h"

namespace Flameberry {
    class EditorLayer
    {
    public:
        static void OnAttach();
        static void OnDetach();
        static void OnImGuiRender();
        static void SetDarkThemeColors();
    private:
        static void OnImGuiBegin();
        static void OnImGuiEnd();
    private:
        static std::shared_ptr<Renderer> s_CoreRenderer;
        static uint32_t s_ViewportWidth, s_ViewportHeight;
    };
}