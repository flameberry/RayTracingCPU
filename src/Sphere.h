#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Hittable.h"

namespace Flameberry {
    class Sphere
    {
    public:
        Sphere(const glm::vec3& center, float radius, const glm::vec3& color = { 1, 0, 1 });
        ~Sphere();

        // returns sphere color if ray has hit the sphere else returns black
        glm::vec4 Hit(Ray& ray);
        void SetColor(const glm::vec3& color) { m_SphereColor = color; }
    private:
        glm::vec3 m_Center;
        float m_Radius;
        glm::vec3 m_SphereColor;
    };
}