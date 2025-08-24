#pragma once

#include "../src/Entity/Entity.h"
#include "Material/Material.h"
#include "Mesh/IndexedMesh/MeshIndexed.h"
#include "AABB.h"
#include <vector>

class GraficosEngine_API Entity3D : public Entity {
private:
    std::vector<Mesh*> meshes;
    Material material;
    AABB localAABB;
    AABB worldAABB;

    void RenderRecursive(
        Camera* camera,
        const std::vector<DirectionalLight>& dirLights,
        const std::vector<PointLight>& pointLights,
        const std::vector<SpotLight>& spotLights
    );

public:
    Entity3D();
    ~Entity3D() override;

    void AddMesh(Mesh* m);
    Mesh* GetMesh() const {
        return meshes.empty() ? nullptr : meshes[0];
    }

    const std::vector<Mesh*>& GetMeshes() const { return meshes; }
    const AABB& GetWorldAABB() const { return worldAABB; }

    void SetMaterialToMeshes(bool recursive = false);
    void SetMaterial(const Material& mat) { material = mat; }
    const Material& GetMaterial() const { return material; }

    void UpdateModel(bool isModelCreated) override;

    void Render(
        Camera* camera,
        const std::vector<DirectionalLight>& dirLights,
        const std::vector<PointLight>& pointLights,
        const std::vector<SpotLight>& spotLights
    )override ;
};
