#include "Camera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Input.h"
namespace Flameberry {
    Camera::Camera(const CameraInfo& cameraInfo)
        : m_AspectRatio(cameraInfo.aspectRatio),
        m_VerticalFOV(cameraInfo.verticalFOV),
        m_CameraOrigin(cameraInfo.cameraOrigin),
        m_ForwardDirection(glm::normalize(cameraInfo.cameraDirection)),
        m_UpDir(cameraInfo.upDir)
    {
        Invalidate();
    }

    void Camera::OnUpdate(float delta)
    {
        glm::vec2 mousePos = Input::GetCursorPosition();
        glm::vec2 rotationDelta = (mousePos - m_LastMousePosition) * 0.002f;
        m_LastMousePosition = mousePos;

        if (!Input::IsMouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS))
        {
            Input::SetCursorMode(GLFW_CURSOR_NORMAL);
            return;
        }

        Input::SetCursorMode(GLFW_CURSOR_DISABLED);

        bool moved = false;
        float rotationSpeed = 0.6f, speed = 3.0f;

        if (Input::IsKey(GLFW_KEY_W, GLFW_PRESS))
        {
            m_CameraOrigin += m_ForwardDirection * speed * delta;
            moved = true;
        }
        if (Input::IsKey(GLFW_KEY_S, GLFW_PRESS))
        {
            m_CameraOrigin -= m_ForwardDirection * speed * delta;
            moved = true;
        }
        if (Input::IsKey(GLFW_KEY_D, GLFW_PRESS))
        {
            m_CameraOrigin += m_RightDirection * speed * delta;
            moved = true;
        }
        if (Input::IsKey(GLFW_KEY_A, GLFW_PRESS))
        {
            m_CameraOrigin -= m_RightDirection * speed * delta;
            moved = true;
        }
        if (Input::IsKey(GLFW_KEY_Q, GLFW_PRESS))
        {
            m_CameraOrigin += m_UpDir * speed * delta;
            moved = true;
        }
        if (Input::IsKey(GLFW_KEY_E, GLFW_PRESS))
        {
            m_CameraOrigin -= m_UpDir * speed * delta;
            moved = true;
        }

        if (rotationDelta.x != 0.0f || rotationDelta.y != 0.0f)
        {
            float pitchDelta = rotationDelta.y * rotationSpeed;
            float yawDelta = rotationDelta.x * rotationSpeed;

            m_RightDirection = glm::cross(m_UpDir, -m_ForwardDirection);
            glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, m_RightDirection),
                glm::angleAxis(-yawDelta, glm::vec3(0.0f, 1.0f, 0.0f))));
            m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

            moved = true;
        }

        if (moved)
            Invalidate();
    }

    void Camera::Invalidate()
    {
        m_ViewportSize.y = 2 * tan(glm::radians(m_VerticalFOV) / 2.0f);
        m_ViewportSize.x = m_AspectRatio * m_ViewportSize.y;

        m_RightDirection = glm::cross(m_UpDir, -m_ForwardDirection);

        m_Horizontal = m_RightDirection * m_ViewportSize.x;
        m_Vertical = m_UpDir * m_ViewportSize.y;

        m_BottomLeft = m_CameraOrigin - m_Horizontal / 2.0f - m_Vertical / 2.0f + m_ForwardDirection;
    }

    Ray Camera::GetRay(float s, float t) const
    {
        return Ray(m_CameraOrigin, m_BottomLeft + s * m_Horizontal + t * m_Vertical - m_CameraOrigin);
    }

    Camera::~Camera()
    {
    }
}