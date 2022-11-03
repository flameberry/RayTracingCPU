#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Camera.h"
#include "Scene.h"

namespace Flameberry {
    struct HitPayload
    {
        float HitDistance;
        uint32_t ObjectIndex;
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
        void Render(const glm::vec2& imageSize, Scene* scene);
        uint32_t GetRenderImageTextureId() { return m_RenderImageTextureId; }
        glm::vec2& GetRenderImageSize() { return m_RenderImageSize; }

        void SetActiveScene(Scene* activeScene) { m_ActiveScene = activeScene; }
        void SetActiveCamera(Camera* activeCamera) { m_ActiveCamera = activeCamera; }
    private:
        uint32_t PerPixel(int x, int y);
    private:
        glm::vec3 m_CameraPos, m_BottomLeft;
        float m_AspectRatio;
    private:
        uint32_t  m_RenderImageTextureId;
        uint32_t* m_RenderImageData;
        glm::vec2 m_RenderImageSize;

        glm::vec2 m_ViewportSize;
        Scene* m_ActiveScene;
        Camera* m_ActiveCamera;
    };
}