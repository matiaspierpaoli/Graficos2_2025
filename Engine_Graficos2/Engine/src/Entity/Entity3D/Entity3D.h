#pragma once

#include "../src/Entity/Entity.h"
#include "../src/Mesh/Mesh.h" 

class GraficosEngine_API Entity3D : public Entity {
private:
    Mesh* mesh;

public:
    Entity3D();
    virtual ~Entity3D();

    void SetMesh(Mesh* newMesh);
    Mesh* GetMesh();

    void UpdateModel(bool isModelCreated);
    void Draw(const glm::mat4& view, const glm::mat4& proj);
};
