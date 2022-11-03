#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

namespace Flameberry {
    class Sphere
    {
    public:
        Sphere(const glm::vec3& center, float radius, const glm::vec3& color = { 1, 0, 1 });
        ~Sphere();

        bool Hit(Ray& ray, glm::vec4& outColor, float& closest_t) const;
        void SetColor(const glm::vec3& color) { m_SphereColor = color; }
    private:
        glm::vec3 m_Center;
        float m_Radius;
        glm::vec3 m_SphereColor;
    };
}