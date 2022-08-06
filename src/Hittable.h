#pragma once
#include "Ray.h"

namespace Flameberry {
    class Hittable
    {
    public:
        Hittable() = default;
        virtual ~Hittable() = default;
        virtual glm::vec4 Hit(Ray& ray) = 0;
    };
}