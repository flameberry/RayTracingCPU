#pragma once
#include <glm/glm.hpp>

#define UNIT(vector) vector /  sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z)

namespace Flameberry
{
    class Ray
    {
    public:
        Ray(const glm::vec3& origin, const glm::vec3& direction);
        ~Ray();

        glm::vec3& origin() { return m_Origin; }
        glm::vec3& direction() { return m_Direction; }
    private:
        glm::vec3 m_Origin, m_Direction;
        float m_T;
    };
}