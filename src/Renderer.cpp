#include "Renderer.h"
#include <glad/glad.h>
#include "Sphere.h"
#include <iostream>

#define YELLOW 0xffff00ff
#define PINK   0xff00ffff
#define BLACK  0x000000ff

void printVec(const glm::vec3& vec)
{
    std::cout << "( " << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
}

uint32_t ToHex(const glm::vec4& color_in_vec4)
{
    uint32_t hex = ((uint32_t)(color_in_vec4.r * 255) << 24) + ((uint32_t)(color_in_vec4.g * 255) << 16) + ((uint32_t)(color_in_vec4.b * 255) << 8) + (uint32_t)(color_in_vec4.a * 255);
    return hex;
}

namespace Flameberry {
    Renderer::Renderer()
        : m_RenderImageTextureId(0), m_RenderImageData(nullptr), m_RenderImageSize(500, 500), m_CameraPos(0.0f, 0.0f, 1.0f)
    {
        m_AspectRatio = m_RenderImageSize.x / m_RenderImageSize.y;
        m_BottomLeft = { -m_AspectRatio, -1.0f, -1.0f };
        glGenTextures(1, &m_RenderImageTextureId);
    }

    Renderer::~Renderer()
    {
        glDeleteTextures(1, &m_RenderImageTextureId);
    }

    uint32_t Renderer::PerPixel(int x, int y)
    {
        float closest_t = FLT_MAX;
        bool hitAnything = false;

        glm::vec4 pixelColor{ 1.0f };
        Ray ray = m_ActiveCamera->GetRay((float)x / (m_RenderImageSize.x - 1.0f), (float)y / (m_RenderImageSize.y - 1.0f));
        for (const auto& sphere : m_ActiveScene->Spheres)
        {
            if (sphere.Hit(ray, pixelColor, closest_t))
                hitAnything = true;
        }
        if (hitAnything)
            return ToHex(pixelColor);
        return BLACK;
    }

    void Renderer::Render(const glm::vec2& imageSize, Scene* scene)
    {
        m_ActiveScene = scene;
        m_RenderImageSize = imageSize;
        m_RenderImageData = new uint32_t[m_RenderImageSize.x * m_RenderImageSize.y];
        m_AspectRatio = m_RenderImageSize.x / m_RenderImageSize.y;

        // Working with pixel data
        for (uint32_t y = 0; y < m_RenderImageSize.y; y++)
        {
            for (uint32_t x = 0; x < m_RenderImageSize.x; x++)
                m_RenderImageData[(size_t)(x + y * m_RenderImageSize.x)] = PerPixel(x, y);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RenderImageTextureId);

        // Set parameters to determine how the texture is resized
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Set parameters to determine how the texture wraps at edges
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_RenderImageSize.x, m_RenderImageSize.y, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, m_RenderImageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        delete[] m_RenderImageData;
    }
}
