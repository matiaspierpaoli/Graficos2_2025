#pragma once

#include "../src/Entity/Entity.h"
#include "../src/Mesh/Mesh.h" 

class GraficosEngine_API Entity3D : public Entity
{
private:
    Mesh* mesh;
    Material material;

public:
    Entity3D();
    ~Entity3D();

    void SetMesh(Mesh* mesh);
    Mesh* GetMesh() const;
    Material GetMaterial() const { return material; }

    void SetMaterial(Material MeshMaterial) { material = MeshMaterial; }

    void UpdateModel(bool isModelCreated) override;
};
