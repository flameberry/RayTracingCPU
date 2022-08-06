#pragma once
#include <glm/glm.hpp>

namespace Flameberry {
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