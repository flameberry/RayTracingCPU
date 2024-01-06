#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <random>

#include "Camera.h"
#include "Scene.h"

namespace Flameberry {
    struct HitPayload
    {
        float HitDistance;
        glm::vec3 WorldPosition, WorldNormal;
        uint32_t ObjectIndex;
    };

    class Renderer
    {
    public:
        struct Settings
        {
            bool Accumulate = false;
            bool MultiThread = false;
        };
    public:
        Renderer();
        ~Renderer();
        void Render(const glm::vec2& imageSize, Scene* scene);
        uint32_t GetRenderImageTextureId() { return m_RenderImageTextureId; }
        glm::vec2& GetRenderImageSize() { return m_RenderImageSize; }

        void SetActiveScene(Scene* activeScene) { m_ActiveScene = activeScene; }
        void SetActiveCamera(Camera* activeCamera) { m_ActiveCamera = activeCamera; }

        Settings& GetSettings() { return m_RenderSettings; }
        void ResetFrameIndex() { m_FrameIndex = 1; }
    private:
        glm::vec4 PerPixel(int x, int y);
        HitPayload TraceRay(const Ray& ray);
    private:
        float m_AspectRatio;
        uint32_t  m_RenderImageTextureId;
        uint32_t* m_RenderImageData;
        glm::vec4* m_AccumulatedRenderImageData;
        glm::vec2 m_RenderImageSize;

        std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

        Settings m_RenderSettings;
        uint32_t m_FrameIndex = 1;

        glm::vec2 m_ViewportSize;
        Scene* m_ActiveScene;
        Camera* m_ActiveCamera;

        std::default_random_engine m_RandomEngine;
        std::mt19937 m_E2;
    };
}