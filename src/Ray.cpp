#include "Ray.h"

namespace Flameberry
{
    Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
        : m_Origin(origin), m_Direction(glm::normalize(direction))
    {
    }

    Ray::~Ray()
    {
    }
}