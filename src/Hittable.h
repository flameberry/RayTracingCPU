#pragma once
#include "Ray.h"

namespace Flameberry {
    class Hittable
    {
    public:
        Hittable() = default;
        virtual ~Hittable() = default;
        virtual bool Hit(Ray& ray, glm::vec4& outColor) = 0;
    };
}