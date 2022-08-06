#include "Application.h"
#include <imgui.h>
#include <iostream>
#include <glad/glad.h>
#include "Sphere.h"

namespace Flameberry {
    Application* Application::s_App;
    Application::Application()
        : m_ViewportWidth(500), m_ViewportHeight(500)
    {
        s_App = this;
        m_ImGuiLayer.OnAttach();
        // m_CoreRenderer.Render({ m_ViewportWidth, m_ViewportHeight });
    }

    Application::~Application()
    {
        m_ImGuiLayer.OnDetach();
        glfwTerminate();
    }

    void Application::Run()
    {
        bool shouldRender = false;
        while (m_Window.IsRunning())
        {
            m_ImGuiLayer.OnBegin();
            ImGui::Begin("Settings");
            // ImGui::Text("Last render: %.3fms", duration * 1000.0f);
            if (ImGui::Button("Render"))
                shouldRender = true;
            else
                shouldRender = false;
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");
            if (shouldRender)
            {
                m_ViewportWidth = ImGui::GetContentRegionAvail().x;
                m_ViewportHeight = ImGui::GetContentRegionAvail().y;
                m_CoreRenderer.Render({ m_ViewportWidth, m_ViewportHeight });
            }
            ImGui::Image(
                (ImTextureID)static_cast<uintptr_t>(m_CoreRenderer.GetRenderImageTextureId()),
                ImVec2((float)m_CoreRenderer.GetRenderImageSize().x, (float)m_CoreRenderer.GetRenderImageSize().y),
                ImVec2{ 0, 1 },
                ImVec2{ 1, 0 }
            );
            ImGui::End();
            ImGui::PopStyleVar();

            m_ImGuiLayer.OnEnd();
            m_Window.OnUpdate();
        }
    }
}