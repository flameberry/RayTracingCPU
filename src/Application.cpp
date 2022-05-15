#include "Application.h"
#include <imgui.h>
#include <iostream>
#include <glad/glad.h>

namespace Flameberry {
    uint32_t CreateTexture()
    {
        uint32_t* data = new uint32_t[100 * 100];
        for (uint32_t i = 0; i < 100 * 100; i++)
            data[i] = 0xffff00ff;

        uint32_t textureId;
        glGenTextures(1, &textureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set parameters to determine how the texture is resized
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Set parameters to determine how the texture wraps at edges
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 100, 100, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return textureId;
    }

    Application* Application::s_App;
    Application::Application()
    {
        s_App = this;
        m_ImGuiLayer.OnAttach();
    }

    void Application::Run()
    {
        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        uint32_t textureId = CreateTexture();

        while (m_Window.IsRunning())
        {
            m_ImGuiLayer.OnBegin();

            ImGui::Begin("Viewport");
            ImGui::Image((ImTextureID)static_cast<uintptr_t>(textureId), ImVec2(100.0f, 100.0f));
            ImGui::End();

            ImGui::Begin("Settings");
            if (ImGui::Button("Render"))
                std::cout << "Rendering...\n";
            ImGui::End();

            m_ImGuiLayer.OnEnd();
            m_Window.OnUpdate();
        }
    }

    Application::~Application()
    {
        m_ImGuiLayer.OnDetach();
        glfwTerminate();
    }
}