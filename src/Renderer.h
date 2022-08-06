#pragma once
#include <glm/glm.hpp>

namespace Flameberry
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
        void Render(const glm::vec2& imageSize);
        uint32_t GetRenderedPixelColor(int x, int y);
        uint32_t GetRenderImageTextureId() { return m_RenderImageTextureId; }
        glm::vec2& GetRenderImageSize() { return m_RenderImageSize; }
    private:
        glm::vec4 GetPixelColor();
    private:
        glm::vec3 m_CameraPos, m_BottomLeft;
        float m_AspectRatio;
    private:
        uint32_t  m_RenderImageTextureId;
        uint32_t* m_RenderImageData;
        glm::vec2 m_RenderImageSize;
    };
}