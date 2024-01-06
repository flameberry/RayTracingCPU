#pragma once
#include "../Renderer.h"

namespace Flameberry {
    class EditorLayer
    {
    public:
        EditorLayer();
        ~EditorLayer();
        void OnAttach();
        void OnDetach();
        void OnImGuiRender();
        void SetDarkThemeColors();
        void OnUpdate(float delta);
    private:
        void OnImGuiBegin();
        void OnImGuiEnd();
    private:
        std::shared_ptr<Renderer> m_CoreRenderer;
        uint32_t m_ViewportWidth, m_ViewportHeight;

        Scene m_ActiveScene;
        Camera m_ActiveCamera;
    };
}