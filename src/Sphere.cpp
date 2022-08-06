#include <iostream>
#include "Sphere.h"
#include <math.h>

namespace Flameberry
{
    Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color)
        : m_Center(center), m_Radius(radius), m_SphereColor(color)
    {
    }

    bool Sphere::Hit(Ray& ray, glm::vec4& outColor, float& closest_t)
    {
        static float a, b, c, determinant;
        a = glm::dot(ray.direction(), ray.direction());
        b = 2 * glm::dot(ray.direction(), ray.origin() - m_Center);
        c = glm::dot(ray.origin() - m_Center, ray.origin() - m_Center) - m_Radius * m_Radius;

        determinant = b * b - 4 * a * c;

        // return if ray doesn't hit sphere
        if (determinant < 0)
            return false;

        // float t = (-b + sqrt(determinant)) / (2.0f * a);
        float t = (-b - sqrt(determinant)) / (2.0f * a);

        if (t > closest_t || t < 0)
            return false;
        closest_t = t;

        glm::vec3 hitPoint = ray.origin() + t * ray.direction();

        glm::vec3 normal = glm::normalize(hitPoint - m_Center);
        glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));

        float d = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(x) // x = angle between normal and -lightDir
        outColor = { m_SphereColor * d, 1.0f };
        return true;
    }

    Sphere::~Sphere()
    {
    }

    std::shared_ptr<Sphere> Sphere::Create(const glm::vec3& center, float radius, const glm::vec3& color)
    {
        return std::make_shared<Sphere>(center, radius, color);
    }
}
