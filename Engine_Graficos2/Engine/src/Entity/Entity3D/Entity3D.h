#pragma once

#include "../src/Entity/Entity.h"
#include "../src/Mesh/Mesh.h" 

class GraficosEngine_API Entity3D : public Entity
{
private:
    Mesh* mesh;

public:
    Entity3D();
    ~Entity3D();

    void SetMesh(Mesh* mesh);
    Mesh* GetMesh() const;
    void UpdateModel(bool isModelCreated) override;
};
