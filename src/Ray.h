#pragma once
#include <glm/glm.hpp>

namespace Flameberry {
    struct Ray
    {
        glm::vec3 Origin, Direction;
        float T;

        Ray(const glm::vec3& origin, const glm::vec3& direction)
            : Origin(origin), Direction(glm::normalize(direction))
        {
        }
    };
}