#include <iostream>
#include "Sphere.h"
#include <math.h>

namespace Flameberry
{
    Sphere::Sphere(const glm::vec3& center, float radius)
        : m_Center(center), m_Radius(radius)
    {
    }

    Intersection<2> Sphere::Hit(Ray& ray)
    {
        static float a, b, c, determinant;
        a = ray.direction().x * ray.direction().x + ray.direction().y * ray.direction().y + ray.direction().z * ray.direction().z;
        b = 2 * ray.direction().x * (ray.origin().x - m_Center.x) + 2 * ray.direction().y * (ray.origin().y - m_Center.y) + 2 * ray.direction().z * (ray.origin().z - m_Center.z);
        c = (ray.origin().x - m_Center.x) * (ray.origin().x - m_Center.x) + (ray.origin().y - m_Center.y) * (ray.origin().y - m_Center.y) + (ray.origin().z - m_Center.z) * (ray.origin().z - m_Center.z) - m_Radius * m_Radius;

        determinant = b * b - 4 * a * c;
        if (determinant > 0)
        {
            float t1 = (-b + sqrt(determinant)) / (2.0f * a);
            float t2 = (-b - sqrt(determinant)) / (2.0f * a);
            glm::vec3 intersection_one = { ray.origin().x + t1 * ray.direction().x, ray.origin().y + t1 * ray.direction().y , ray.origin().z + t1 * ray.direction().z };
            glm::vec3 intersection_two = { ray.origin().x + t2 * ray.direction().x, ray.origin().y + t2 * ray.direction().y , ray.origin().z + t2 * ray.direction().z };;
            // std::cout << "Ray Has Hit the sphere twice!" << std::endl;
            // printf("The Ray has hit the sphere at points (%f, %f, %f) and (%f, %f, %f)!", intersection_one.x, intersection_one.y, intersection_one.z, intersection_two.x, intersection_two.y, intersection_two.z);
            return { true, {intersection_one, intersection_two} };
        }
        else if (!determinant)
        {
            float t = -b / (2.0f * a);
            glm::vec3 intersection = { ray.origin().x + t * ray.direction().x, ray.origin().y + t * ray.direction().y , ray.origin().z + t * ray.direction().z };
            // std::cout << "Ray is tangent to sphere!" << std::endl;
            // printf("The Ray has hit the sphere at the point (%f, %f, %f)!", intersection.x, intersection.y, intersection.z);
            return { true, {intersection, intersection} };
        }
        else if (determinant < 0)
        {
            // std::cout << "The ray hasn't hit the sphere!" << std::endl;
            return { false };
        }
    }

    glm::vec3 Sphere::GetNormalAt(const glm::vec3& point)
    {
        return UNIT(point - m_Center);
    }

    Sphere::~Sphere()
    {
    }
}