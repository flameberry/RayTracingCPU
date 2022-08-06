#pragma once
#include <glm/glm.hpp>
#include "../Camera.h"

namespace Flameberry {
    class CameraSettingsPanel
    {
    public:
        CameraSettingsPanel() = default;
        CameraSettingsPanel(const CameraInfo& cameraInfo);
        ~CameraSettingsPanel();
        const glm::vec3& GetCameraPosition() const { return m_CameraInfo.cameraOrigin; }
        float GetCameraFOV() const { return m_CameraInfo.verticalFOV; }
        void OnImGuiRender();
    private:
        CameraInfo m_CameraInfo;
    };
}