#pragma once

#include <vector>
#include "Sphere.h"

namespace Flameberry {
    struct Scene
    {
        std::vector<Sphere> Spheres;
        std::vector<Material> Materials;
    };
}