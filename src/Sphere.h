#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

namespace Flameberry {
    struct Material
    {
        glm::vec3 Albedo{ 1.0f };
        float Roughness = 0.0f;
        glm::vec3 EmissionColor{ 1.0f };
        float EmissionPower = 0.0f;
    };

    struct Sphere
    {
        glm::vec3 Center;
        float Radius;
        int MaterialIndex;
    };
}