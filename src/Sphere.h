#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Hittable.h"

namespace Flameberry {
    class Sphere : public Hittable
    {
    public:
        static std::shared_ptr<Sphere> Create(const glm::vec3& center, float radius, const glm::vec3& color = { 1, 0, 1 });
        Sphere(const glm::vec3& center, float radius, const glm::vec3& color = { 1, 0, 1 });
        ~Sphere();

        bool Hit(Ray& ray, glm::vec4& outColor) override;
        void SetColor(const glm::vec3& color) { m_SphereColor = color; }
    private:
        glm::vec3 m_Center;
        float m_Radius;
        glm::vec3 m_SphereColor;
    };
}