#include "Renderer.h"

#include <iostream>
#include <glad/glad.h>

#include "Sphere.h"
#include "Random.h"

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
        : m_RenderImageTextureId(0), m_RenderImageData(nullptr), m_AccumulatedRenderImageData(nullptr), m_RenderImageSize(500, 500)
    {
        m_AspectRatio = m_RenderImageSize.x / m_RenderImageSize.y;
        glGenTextures(1, &m_RenderImageTextureId);
    }

    Renderer::~Renderer()
    {
        glDeleteTextures(1, &m_RenderImageTextureId);
    }

    glm::vec4 Renderer::PerPixel(int x, int y)
    {
        glm::vec3 colorContrib(1.0f);
        glm::vec3 finalColor{ 0.0f };

        Ray ray = m_ActiveCamera->GetRay((float)x / m_RenderImageSize.x, (float)y / m_RenderImageSize.y);

        int bounces = 5;
        for (uint32_t i = 0; i < bounces; i++)
        {
            HitPayload payload = TraceRay(ray);

            if (payload.HitDistance < 0.0f)
            {
                glm::vec3 skyColor{ 0.6f, 0.7f, 0.9f };
                // glm::vec3 skyColor{ 0.0f, 0.0f, 0.0f };
                finalColor += skyColor * colorContrib;
                break;
            }

            glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
            float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f); // == cos(x) // x = angle between normal and -lightDir

            Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
            Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];

            colorContrib *= material.Albedo;
            // finalColor += material.Albedo * lightIntensity * colorContrib;
            finalColor += material.EmissionColor * material.EmissionPower;

            ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;

            // ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal + material.Roughness * randomDir);
            ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal + Random::InUnitSphere());
        }
        return { finalColor, 1.0f };
    }

    HitPayload Renderer::TraceRay(const Ray& ray)
    {
        float closest_t = FLT_MAX;
        bool hitAnything = false;
        uint32_t sphereIndex;

        for (uint32_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
        {
            Sphere& sphere = m_ActiveScene->Spheres[i];
            float a, b, c, determinant;
            a = glm::dot(ray.Direction, ray.Direction);
            b = 2 * glm::dot(ray.Direction, ray.Origin - sphere.Center);
            c = glm::dot(ray.Origin - sphere.Center, ray.Origin - sphere.Center) - sphere.Radius * sphere.Radius;

            determinant = b * b - 4 * a * c;

            // return if ray doesn't hit sphere
            if (determinant < 0)
                continue;

            float t = (-b - glm::sqrt(determinant)) / (2.0f * a);
            if (t < 0.0f || t > closest_t)
                continue;

            closest_t = t;
            sphereIndex = i;
            hitAnything = true;
        }

        if (hitAnything && closest_t > 0.0f)
        {
            HitPayload payload;
            payload.WorldPosition = ray.Origin + closest_t * ray.Direction;
            payload.HitDistance = closest_t;
            payload.WorldNormal = glm::normalize(payload.WorldPosition - m_ActiveScene->Spheres[sphereIndex].Center);
            payload.ObjectIndex = sphereIndex;
            return payload;
        }
        return HitPayload{ .HitDistance = -1.0f };
    }

    void Renderer::Render(const glm::vec2& imageSize, Scene* scene)
    {
        m_ActiveScene = scene;
        bool allocated = false;
        if (!m_RenderImageData || m_RenderImageSize != imageSize)
        {
            m_RenderImageSize = imageSize;
            m_RenderImageData = new uint32_t[m_RenderImageSize.x * m_RenderImageSize.y];
            m_AccumulatedRenderImageData = new glm::vec4[m_RenderImageSize.x * m_RenderImageSize.y];
            allocated = true;

            if (m_RenderSettings.MultiThread)
            {
                m_ImageHorizontalIter.resize(imageSize.x);
                m_ImageVerticalIter.resize(imageSize.y);

                for (uint32_t i = 0; i < imageSize.x; i++)
                    m_ImageHorizontalIter[i] = i;
                for (uint32_t i = 0; i < imageSize.y; i++)
                    m_ImageVerticalIter[i] = i;
            }
        }
        m_AspectRatio = m_RenderImageSize.x / m_RenderImageSize.y;

        if (m_FrameIndex == 1)
            memset(m_AccumulatedRenderImageData, 0, m_RenderImageSize.x * m_RenderImageSize.y * sizeof(glm::vec4));

        if (m_RenderSettings.MultiThread)
            assert(0);
        else
        {
            // Working with pixel data
            for (uint32_t y = 0; y < m_RenderImageSize.y; y++)
            {
                for (uint32_t x = 0; x < m_RenderImageSize.x; x++)
                {
                    glm::vec4 color = PerPixel(x, y);
                    m_AccumulatedRenderImageData[(size_t)(x + y * m_RenderImageSize.x)] += color;

                    glm::vec4 accumulatedColor = m_AccumulatedRenderImageData[(size_t)(x + y * m_RenderImageSize.x)] / (float)m_FrameIndex;
                    accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                    m_RenderImageData[(size_t)(x + y * m_RenderImageSize.x)] = ToHex(accumulatedColor);
                }
            }
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

        if (m_RenderSettings.Accumulate)
            m_FrameIndex++;
        else
            m_FrameIndex = 1;

        if (allocated)
            delete[] m_RenderImageData;
    }
}
