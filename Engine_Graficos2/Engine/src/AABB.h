#pragma once
#include <../libs/glm/glm.hpp>

struct AABB {
    glm::vec3 min{ +FLT_MAX }, max{ -FLT_MAX };

    AABB() = default;
    AABB(const glm::vec3& mi, const glm::vec3& ma) : min(mi), max(ma) {}

    void expand(const glm::vec3& p) { min = glm::min(min, p); max = glm::max(max, p); }
    void merge(const AABB& b) { min = glm::min(min, b.min); max = glm::max(max, b.max); }
    bool valid() const { return min.x <= max.x && min.y <= max.y && min.z <= max.z; }
};

inline AABB TransformAABB(const AABB& box, const glm::mat4& M) {
    if (!box.valid()) return box;
    glm::vec3 c[8] = {
        {box.min.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.min.z},
        {box.min.x, box.max.y, box.min.z}, {box.max.x, box.max.y, box.min.z},
        {box.min.x, box.min.y, box.max.z}, {box.max.x, box.min.y, box.max.z},
        {box.min.x, box.max.y, box.max.z}, {box.max.x, box.max.y, box.max.z}
    };
    AABB out;
    for (int i = 0; i < 8; ++i)
        out.expand(glm::vec3(M * glm::vec4(c[i], 1)));
    return out;
}
