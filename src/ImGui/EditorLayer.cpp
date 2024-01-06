#include "EditorLayer.h"

#include <iostream>
#include <chrono>
#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Application.h"
#include "../Sphere.h"

namespace Utils {
    void DrawVec3Control(const std::string& str_id, glm::vec3& value, float defaultValue, float dragSpeed, float availWidth)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGuiIO& io = ImGui::GetIO();
        // auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(str_id.c_str());

        ImGui::PushMultiItemsWidths(3, ceil(availWidth + 7.0f - 3 * buttonSize.x));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

        // ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            value.x = defaultValue;
        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &value.x, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

        // ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            value.y = defaultValue;
        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &value.y, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

        // ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            value.z = defaultValue;
        // ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &value.z, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();

        ImGui::PopID();
        ImGui::PopStyleVar();
    }
}

namespace Flameberry {
    EditorLayer::EditorLayer()
        : m_ViewportWidth(500), m_ViewportHeight(500)
    {
        int maxSamples;
        glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
        std::cout << maxSamples << "\n";

        CameraInfo cameraInfo{};
        cameraInfo.aspectRatio = m_ViewportWidth / m_ViewportHeight;
        cameraInfo.verticalFOV = 32.0f;
        // cameraInfo.verticalFOV = 39.6f;
        cameraInfo.cameraOrigin = glm::vec3(0, 0, 4);
        cameraInfo.cameraDirection = glm::vec3(0, 0, -1);

        m_ActiveCamera = Camera(cameraInfo);

        Material pinkMaterial;
        pinkMaterial.Albedo = { 1.0f, 0.0f, 1.0f };
        Material blueMaterial;
        blueMaterial.Albedo = { 0.2f, 0.3f, 1.0f };
        blueMaterial.Roughness = 1.0f;
        Material yellowEmissive;
        yellowEmissive.Albedo = { 0.8f, 0.5f, 0.2f };
        yellowEmissive.Roughness = 1.0f;
        yellowEmissive.EmissionColor = yellowEmissive.Albedo;
        yellowEmissive.EmissionPower = 1.5f;

        m_ActiveScene.Materials.push_back(pinkMaterial);
        m_ActiveScene.Materials.push_back(blueMaterial);
        m_ActiveScene.Materials.push_back(yellowEmissive);
        {
            Sphere sphere;
            sphere.Center = { 0.0f, 0.0f, 0.0f };
            sphere.Radius = 0.5f;
            sphere.MaterialIndex = 0;
            m_ActiveScene.Spheres.push_back(sphere);
        }
        {
            Sphere sphere;
            sphere.Center = { 0.0f, -100.5f, 0.0f };
            sphere.Radius = 100.0f;
            sphere.MaterialIndex = 1;
            m_ActiveScene.Spheres.push_back(sphere);
        }
        {
            Sphere sphere;
            sphere.Center = { 1.0f, 0.0f, 0.0f };
            sphere.Radius = 0.5f;
            sphere.MaterialIndex = 2;
            m_ActiveScene.Spheres.push_back(sphere);
        }
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // float fontSize = 15.0f;// *2.0f;
        // io.Fonts->AddFontFromFileTTF("/Users/flameberry/Developer/RayTracing/assets/fonts/OpenSans-Regular.ttf", fontSize);
        // io.FontDefault = io.Fonts->AddFontFromFileTTF(FL_PROJECT_DIR"Flameberry/assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

        Application& app = Application::GetApp();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetMainWindow().GetGLFWwindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        m_CoreRenderer = std::make_shared<Renderer>();
        m_CoreRenderer->SetActiveScene(&m_ActiveScene);
        m_CoreRenderer->SetActiveCamera(&m_ActiveCamera);
    }

    void EditorLayer::OnDetach()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorLayer::OnUpdate(float delta)
    {
        if (m_ActiveCamera.OnUpdate(delta))
            m_CoreRenderer->ResetFrameIndex();
    }

    void EditorLayer::OnImGuiRender()
    {
        OnImGuiBegin();

        bool shouldRender = false;
        static float renderTime = 0.0f;
        static bool realTimeRendering = true;

        ImGui::Begin("Scene");
        ImGui::Text("Spheres");
        for (uint32_t i = 0; i < m_ActiveScene.Spheres.size(); i++)
        {
            ImGui::PushID(i);
            auto& sphere = m_ActiveScene.Spheres[i];
            Utils::DrawVec3Control("Position", sphere.Center, 0.0f, 0.005f, ImGui::GetContentRegionAvail().x);
            ImGui::Spacing();
            ImGui::DragFloat("Radius", &sphere.Radius, 0.005f, 0.0f, FLT_MAX);
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::Separator();
        ImGui::Text("Materials");

        for (uint32_t i = 0; i < m_ActiveScene.Materials.size(); i++)
        {
            ImGui::PushID(i);
            ImGui::ColorEdit3("Albedo", glm::value_ptr(m_ActiveScene.Materials[i].Albedo));
            ImGui::DragFloat("Roughness", &m_ActiveScene.Materials[i].Roughness, 0.005f);
            ImGui::ColorEdit3("Emission Color", glm::value_ptr(m_ActiveScene.Materials[i].EmissionColor));
            ImGui::DragFloat("Emission Power", &m_ActiveScene.Materials[i].EmissionPower, 0.05f);
            ImGui::PopID();

            ImGui::Separator();
        }
        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", renderTime * 0.001f * 0.001f);
        ImGui::Checkbox("Real Time Rendering ", &realTimeRendering);
        if (!realTimeRendering)
            shouldRender = ImGui::Button("Render");

        ImGui::Checkbox("Accumulate", &(m_CoreRenderer->GetSettings().Accumulate));
        ImGui::Checkbox("MultiThread", &(m_CoreRenderer->GetSettings().MultiThread));
        if (ImGui::Button("Reset"))
            m_CoreRenderer->ResetFrameIndex();

        ImGui::Separator();
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        if (realTimeRendering || shouldRender)
        {
            auto start = std::chrono::high_resolution_clock::now();

            m_ViewportWidth = ImGui::GetContentRegionAvail().x;
            m_ViewportHeight = ImGui::GetContentRegionAvail().y;

            m_ActiveCamera.OnResize((float)m_ViewportWidth / (float)m_ViewportHeight);

            m_CoreRenderer->Render({ m_ViewportWidth, m_ViewportHeight }, &m_ActiveScene);
            renderTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
        }
        ImGui::Image(
            (ImTextureID)static_cast<uintptr_t>(m_CoreRenderer->GetRenderImageTextureId()),
            ImVec2((float)m_CoreRenderer->GetRenderImageSize().x, (float)m_CoreRenderer->GetRenderImageSize().y),
            ImVec2{ 0, 1 },
            ImVec2{ 1, 0 }
        );
        ImGui::End();
        ImGui::PopStyleVar();

        OnImGuiEnd();
    }

    void EditorLayer::OnImGuiBegin()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }

    void EditorLayer::OnImGuiEnd()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetApp();
        io.DisplaySize = ImVec2((float)app.GetMainWindow().GetWidth(), (float)app.GetMainWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void EditorLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }
}
