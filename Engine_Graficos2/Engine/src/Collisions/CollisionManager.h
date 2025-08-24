#pragma once
#include "Exports.h"

#include "Entity/Entity3D/Entity3D.h"
#include "AABB.h"

#include <../libs/glm/glm.hpp>

class GraficosEngine_API CollisionManager
{
public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    static bool IntersectAABB3D(const AABB& a, const AABB& b);
    static bool IntersectAABB3D(const Entity3D* a, const Entity3D* b);

    static glm::vec3 ComputeMTD(const AABB& a, const AABB& b);

    static void ResolveSimple(Entity3D* a, Entity3D* b);
};
