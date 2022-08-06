#include <iostream>
#include "Sphere.h"
#include <math.h>

namespace Flameberry
{
    Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color)
        : m_Center(center), m_Radius(radius), m_SphereColor(color)
    {
    }

    glm::vec4 Sphere::Hit(Ray& ray)
    {
        static float a, b, c, determinant;
        a = glm::dot(ray.direction(), ray.direction());
        b = 2 * glm::dot(ray.direction(), ray.origin() - m_Center);
        c = glm::dot(ray.origin() - m_Center, ray.origin() - m_Center) - m_Radius * m_Radius;

        determinant = b * b - 4 * a * c;

        // return if ray doesn't hit sphere
        if (determinant < 0)
            return glm::vec4(0, 0, 0, 1);

        float t1 = (-b + sqrt(determinant)) / (2.0f * a);
        float t2 = (-b - sqrt(determinant)) / (2.0f * a);

        glm::vec3 intersection_farther = ray.origin() + t1 * ray.direction();
        glm::vec3 intersection_closer = ray.origin() + t2 * ray.direction();

        glm::vec3 normal = glm::normalize(intersection_closer - m_Center);

        glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));

        float d = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(x) // x = angle between normal and -lightDir
        m_SphereColor *= d;
        return { m_SphereColor, 1.0f };
    }

    Sphere::~Sphere()
    {
    }
}