#pragma once
#include "Vector3.h"
#include "../libs/glm/glm.hpp"

inline glm::vec3 ToGLM(const Vector3& v) {
    return glm::vec3(v.x, v.y, v.z);
}
