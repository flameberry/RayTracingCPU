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
    uint32_t hex = ((((int)color_in_vec4.r * 255) & 0xff) << 24) + ((((int)color_in_vec4.g * 255) & 0xff) << 16) + ((((int)color_in_vec4.b * 255) & 0xff) << 8) + (((int)color_in_vec4.a * 255) & 0xff);
    return hex;
}

namespace Flameberry
{
    Renderer::Renderer()
        : m_RenderImageTextureId(0), m_RenderImageData(nullptr), m_RenderImageSize(500, 500), m_CameraPos(0.0f)
    {
        m_AspectRatio = m_RenderImageSize.x / m_RenderImageSize.y;
        m_BottomLeft = { -m_AspectRatio, -1.0f, -1.0f };
        glGenTextures(1, &m_RenderImageTextureId);
    }

    Renderer::~Renderer()
    {
        glDeleteTextures(1, &m_RenderImageTextureId);
    }

    uint32_t Renderer::GetRenderedPixelColor(int x, int y)
    {
        Flameberry::Ray ray(m_CameraPos, m_BottomLeft + glm::vec3{ x * 2.0f * m_AspectRatio / (m_RenderImageSize.x - 1.0f), y * 2.0f / (m_RenderImageSize.y - 1.0f), -1.0f });
        Sphere sphere({ 0.0f, 0.0f, -1.0f }, 0.3f);
        auto intersection = sphere.Hit(ray);

        if (intersection.did_hit)
            return PINK;
        else
            return BLACK;
    }

    void Renderer::Render(const glm::vec2& imageSize)
    {
        m_RenderImageSize = imageSize;
        m_RenderImageData = new uint32_t[m_RenderImageSize.x * m_RenderImageSize.y];
        m_AspectRatio = m_RenderImageSize.x / m_RenderImageSize.y;
        m_BottomLeft = { -m_AspectRatio, -1.0f, -1.0f };

        // Working with pixel data
        uint32_t x = 0, y = 0;
        for (uint32_t i = 0; i < m_RenderImageSize.x * m_RenderImageSize.y; i++)
        {
            if (i == m_RenderImageSize.x * (y + 1))
            {
                y++;
                x = 0;
            }

            m_RenderImageData[i] = GetRenderedPixelColor(x, y);
            x++;
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