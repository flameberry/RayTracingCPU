#include "Camera.h"

namespace Flameberry {
    Camera::Camera(const CameraInfo& ccameraInfo)
        : m_AspectRatio(ccameraInfo.aspectRatio),
        m_VerticalFOV(ccameraInfo.verticalFOV),
        m_CameraOrigin(ccameraInfo.cameraOrigin),
        m_CameraDirection(glm::normalize(ccameraInfo.cameraDirection)),
        m_UpDir(ccameraInfo.upDir)
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