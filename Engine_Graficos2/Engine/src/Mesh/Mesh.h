#pragma once
#include "Exports.h"
#include <../libs/glm/glm.hpp>
#include <vector>
#include "Camera/Camera.h"


class GraficosEngine_API Mesh {
protected:
    unsigned int* vBuffer;
    unsigned int* iBuffer;
    unsigned int modelId;

public:
    Mesh();
    virtual ~Mesh();

    unsigned int GetVertexBuffer() const { return *vBuffer; }
    unsigned int GetIndexBuffer() const { return *iBuffer; }
    unsigned int GetModelId() const { return modelId; }

    void SetModelId(unsigned int id) { modelId = id; }

    virtual void Render(class Camera* camera) = 0;
};
