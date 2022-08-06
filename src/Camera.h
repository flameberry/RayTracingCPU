#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

namespace Flameberry {
    struct CameraInfo
    {
        float aspectRatio, verticalFOV;
        glm::vec3 cameraOrigin, cameraDirection, upDir;
    };

    class Camera
    {
    public:
        Camera() = default;
        Camera(const CameraInfo& ccameraInfo);
        ~Camera();
        void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
        void SetCameraPosition(const glm::vec3& position) { m_CameraOrigin = position; }
        void SetVerticalFOV(float fov) { m_VerticalFOV = fov; }
        Ray GetRay(float s, float t) const;
        void Invalidate();
    private:
    private:
        float m_AspectRatio, m_VerticalFOV;
        glm::vec3 m_CameraOrigin, m_CameraDirection, m_UpDir, m_BottomLeft, m_Horizontal, m_Vertical;
        glm::vec2 m_ViewportSize;
    };
}