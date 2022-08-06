#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

template<int T>
struct Intersection {
    bool did_hit = false;
    glm::vec3 points[T];
};

namespace Flameberry
{
    class Sphere
    {
    public:
        Sphere(const glm::vec3& center, float radius);
        ~Sphere();

        Intersection<2> Hit(Ray& ray);
        glm::vec3 GetNormalAt(const glm::vec3& point);
    private:
        glm::vec3 m_Center;
        float m_Radius;
    };
}