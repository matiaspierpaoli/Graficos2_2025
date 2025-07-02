#pragma once

#include "../src/Entity/Entity.h"
#include "Material/Material.h"
#include <Mesh/IndexedMesh/MeshIndexed.h>
#include <vector>

class GraficosEngine_API Entity3D : public Entity {
private:
    std::vector<Mesh*> meshes;
    Material material;

public:
    Entity3D();
    ~Entity3D();

    void AddMesh(Mesh* m);
    Mesh* GetMesh() const {
        return meshes.empty() ? nullptr : meshes[0];
    }

    void SetMaterial(const Material& mat) { material = mat; }
    const Material& GetMaterial() const { return material; }

    void UpdateModel(bool isModelCreated) override;

    void Render(
        Camera* camera,
        const std::vector<DirectionalLight>& dirLights,
        const std::vector<PointLight>& pointLights,
        const std::vector<SpotLight>& spotLights
    );
};
