#include "Camera.h"

namespace Flameberry {
    Camera::Camera(const CameraInfo& cameraInfo)
        : m_AspectRatio(cameraInfo.aspectRatio),
        m_VerticalFOV(cameraInfo.verticalFOV),
        m_CameraOrigin(cameraInfo.cameraOrigin),
        m_CameraDirection(glm::normalize(cameraInfo.cameraDirection)),
        m_UpDir(cameraInfo.upDir)
    {
        Invalidate();
    }

    void Camera::Invalidate()
    {
        m_ViewportSize.y = 2 * tan(glm::radians(m_VerticalFOV) / 2.0f);
        m_ViewportSize.x = m_AspectRatio * m_ViewportSize.y;

        glm::vec3 rightHorizontalDir = glm::cross(m_UpDir, -m_CameraDirection);

        m_Horizontal = rightHorizontalDir * m_ViewportSize.x;
        m_Vertical = m_UpDir * m_ViewportSize.y;

        m_BottomLeft = m_CameraOrigin - m_Horizontal / 2.0f - m_Vertical / 2.0f + m_CameraDirection;
    }

    Ray Camera::GetRay(float s, float t) const
    {
        return Ray(m_CameraOrigin, m_BottomLeft + s * m_Horizontal + t * m_Vertical - m_CameraOrigin);
    }

    Camera::~Camera()
    {
    }
}