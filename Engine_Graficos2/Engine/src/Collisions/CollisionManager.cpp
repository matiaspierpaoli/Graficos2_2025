#include "CollisionManager.h"
#include <algorithm>

bool CollisionManager::IntersectAABB3D(const AABB& a, const AABB& b)
{
    if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
    if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
    return true;
}

bool CollisionManager::IntersectAABB3D(const Entity3D* a, const Entity3D* b)
{
    return IntersectAABB3D(a->GetWorldAABB(), b->GetWorldAABB());
}

glm::vec3 CollisionManager::ComputeMTD(const AABB& a, const AABB& b)
{
    float dx1 = b.max.x - a.min.x;
    float dx2 = a.max.x - b.min.x;
    float dy1 = b.max.y - a.min.y;
    float dy2 = a.max.y - b.min.y;
    float dz1 = b.max.z - a.min.z;
    float dz2 = a.max.z - b.min.z;

    float px = (dx1 < dx2) ? dx1 : -dx2;
    float py = (dy1 < dy2) ? dy1 : -dy2;
    float pz = (dz1 < dz2) ? dz1 : -dz2;

    float ax = std::abs(px), ay = std::abs(py), az = std::abs(pz);
    if (ax <= ay && ax <= az) return { px, 0.0f, 0.0f };
    if (ay <= ax && ay <= az) return { 0.0f, py, 0.0f };
    return { 0.0f, 0.0f, pz };
}

void CollisionManager::ResolveSimple(Entity3D* a, Entity3D* b)
{
    const AABB& A = a->GetWorldAABB();
    const AABB& B = b->GetWorldAABB();
    if (!IntersectAABB3D(A, B)) return;

    glm::vec3 mtd = ComputeMTD(A, B); // get A from B

    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    else if (a->IsStatic() && !b->IsStatic()) {
        // Push B the other way
        b->Translate(-mtd.x, -mtd.y, -mtd.z);
    }
    else if (!a->IsStatic() && b->IsStatic()) {
        // Push A
        a->Translate(mtd.x, mtd.y, mtd.z);
    }
    else {
        // Half movement to each
        a->Translate(0.5f * mtd.x, 0.5f * mtd.y, 0.5f * mtd.z);
        b->Translate(-0.5f * mtd.x, -0.5f * mtd.y, -0.5f * mtd.z);
    }
}
