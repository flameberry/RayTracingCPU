#include "CameraSettingsPanel.h"
#include <imgui.h>
#include <imgui/imgui_internal.h>

namespace Flameberry {
    CameraSettingsPanel::CameraSettingsPanel(const CameraInfo& cameraInfo)
        : m_CameraInfo(cameraInfo)
    {
    }

    CameraSettingsPanel::~CameraSettingsPanel()
    {
    }

    void CameraSettingsPanel::OnImGuiRender()
    {
        ImGui::Begin("Camera Settings");
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        float dragSpeed = 0.01f;

        ImGui::Text("Camera Position:");
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        if (ImGui::Button("X", buttonSize))
            m_CameraInfo.cameraOrigin.x = 0.0f;
        ImGui::SameLine();
        ImGui::DragFloat("##X", &m_CameraInfo.cameraOrigin.x, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        if (ImGui::Button("Y", buttonSize))
            m_CameraInfo.cameraOrigin.y = 0.0f;
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &m_CameraInfo.cameraOrigin.y, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        if (ImGui::Button("Z", buttonSize))
            m_CameraInfo.cameraOrigin.z = 1.0f;
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &m_CameraInfo.cameraOrigin.z, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
        ImGui::Spacing();
        ImGui::DragFloat("Vertical FOV", &m_CameraInfo.verticalFOV, 0.5f, 0.0f, 180.0f, "%.2f");

        ImGui::End();
    }
}