#include "EditorLayer.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include "../Application.h"
#include <glad/glad.h>
#include "../Sphere.h"
#include <chrono>
#include <imgui/imgui_internal.h>

namespace Flameberry {
    EditorLayer::EditorLayer()
        : m_ViewportWidth(500), m_ViewportHeight(500)
    {
        CameraInfo cameraInfo{};
        cameraInfo.aspectRatio = m_ViewportWidth / m_ViewportHeight;
        cameraInfo.verticalFOV = 60.0f;
        cameraInfo.cameraOrigin = glm::vec3(0, 0, 1);
        cameraInfo.cameraDirection = glm::vec3(0, 0, -1);
        cameraInfo.upDir = glm::vec3(0, 1, 0);

        m_Camera = Camera(cameraInfo);
        m_CameraSettingsPanel = CameraSettingsPanel(cameraInfo);
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

        float fontSize = 15.0f;// *2.0f;
        io.Fonts->AddFontFromFileTTF("/Users/flameberry/Developer/RayTracing/assets/fonts/OpenSans-Regular.ttf", fontSize);
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
    }

    void EditorLayer::OnDetach()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorLayer::OnImGuiRender()
    {
        OnImGuiBegin();

        bool shouldRender = false;
        static float renderTime = 0.0f;
        static bool realTimeRendering = true;

        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", renderTime * 0.001f * 0.001f);
        ImGui::Checkbox("Real Time Rendering ", &realTimeRendering);
        if (!realTimeRendering)
            shouldRender = ImGui::Button("Render");
        ImGui::Separator();
        ImGui::ColorPicker4("Sphere Color", m_SphereColor);
        ImGui::End();

        m_CameraSettingsPanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        if (realTimeRendering || shouldRender)
        {
            auto start = std::chrono::high_resolution_clock::now();

            m_ViewportWidth = ImGui::GetContentRegionAvail().x;
            m_ViewportHeight = ImGui::GetContentRegionAvail().y;

            m_Camera.SetAspectRatio((float)m_ViewportWidth / (float)m_ViewportHeight);
            m_Camera.SetCameraPosition(m_CameraSettingsPanel.GetCameraPosition());
            m_Camera.SetVerticalFOV(m_CameraSettingsPanel.GetCameraFOV());
            m_Camera.Invalidate();

            m_CoreRenderer->Add(Sphere::Create({ 0.0f, 0.0f, 0.0f }, 0.2f, glm::vec4(m_SphereColor[0], m_SphereColor[1], m_SphereColor[2], m_SphereColor[3])));
            m_CoreRenderer->Add(Sphere::Create({ 0.0f, -100.5f, 0.0f }, 100.0f));
            m_CoreRenderer->Render({ m_ViewportWidth, m_ViewportHeight }, m_Camera);

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
