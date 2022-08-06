#pragma once
#include "../Renderer.h"
#include "CameraSettingsPanel.h"

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
    private:
        void OnImGuiBegin();
        void OnImGuiEnd();
    private:
        std::shared_ptr<Renderer> m_CoreRenderer;
        Camera m_Camera;
        CameraSettingsPanel m_CameraSettingsPanel;

        float m_SphereColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
        uint32_t m_ViewportWidth, m_ViewportHeight;
    };
}