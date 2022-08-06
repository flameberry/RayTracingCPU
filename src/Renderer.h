#pragma once
#include <glm/glm.hpp>
#include "Hittable.h"
#include <memory>
#include <vector>
#include "Camera.h"

namespace Flameberry {
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
        void Render(const glm::vec2& imageSize, const Camera& camera);
        uint32_t GetRenderImageTextureId() { return m_RenderImageTextureId; }
        glm::vec2& GetRenderImageSize() { return m_RenderImageSize; }
        void Add(const std::shared_ptr<Hittable>& hittable) { m_HittableObjects.push_back(hittable); }
    private:
        uint32_t CalculatePixelColor(int x, int y, const Camera& camera);
    private:
        glm::vec3 m_CameraPos, m_BottomLeft;
        float m_AspectRatio;
    private:
        uint32_t  m_RenderImageTextureId;
        uint32_t* m_RenderImageData;
        glm::vec2 m_RenderImageSize;

        glm::vec2 m_ViewportSize;

        std::vector<std::shared_ptr<Hittable>> m_HittableObjects;
    };
}