#include "Application.h"
#include <imgui.h>
#include <iostream>
#include <glad/glad.h>

namespace Flameberry {
    Application* Application::s_App;
    Application::Application()
    {
        s_App = this;
        m_ImGuiLayer.OnAttach();
        m_ViewportWidth = 500;
        m_ViewportHeight = 500;

        glGenTextures(1, &m_RenderImageTextureId);
        Render();
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
            {
                shouldRender = true;
                Render();
            }
            else
            {
                shouldRender = false;
            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");
            if (shouldRender)
            {
                m_ViewportWidth = ImGui::GetContentRegionAvail().x;
                m_ViewportHeight = ImGui::GetContentRegionAvail().y;
            }
            ImGui::Image((ImTextureID)static_cast<uintptr_t>(m_RenderImageTextureId), ImVec2((float)m_ViewportWidth, (float)m_ViewportHeight));
            ImGui::End();
            ImGui::PopStyleVar();

            m_ImGuiLayer.OnEnd();
            m_Window.OnUpdate();
        }
    }

    void Application::Render()
    {
        m_RenderImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
        for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
            m_RenderImageData[i] = 0xff00ffff;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RenderImageTextureId);

        // Set parameters to determine how the texture is resized
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Set parameters to determine how the texture wraps at edges
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ViewportWidth, m_ViewportHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, m_RenderImageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        delete[] m_RenderImageData;
    }

    Application::~Application()
    {
        m_ImGuiLayer.OnDetach();
        glDeleteTextures(1, &m_RenderImageTextureId);
        glfwTerminate();
    }
}