#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

namespace Flameberry {
    struct CameraInfo
    {
        float aspectRatio, verticalFOV;
        glm::vec3 cameraOrigin, cameraDirection;
    };

    class Camera
    {
    public:
        Camera() = default;
        Camera(const CameraInfo& cameraInfo);
        ~Camera();
        void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
        void SetCameraPosition(const glm::vec3& position) { m_CameraOrigin = position; }
        void SetVerticalFOV(float fov) { m_VerticalFOV = fov; }
        Ray GetRay(float s, float t) const;
        bool OnUpdate(float delta);
        void OnResize(float aspectRatio);
    private:
        void Invalidate();
    private:
        float m_AspectRatio, m_VerticalFOV;
        glm::vec3 m_CameraOrigin, m_ForwardDirection, m_UpDir, m_RightDirection, m_BottomLeft, m_Horizontal, m_Vertical;
        glm::vec2 m_ViewportSize;

        glm::vec2 m_LastMousePosition;
    };
}