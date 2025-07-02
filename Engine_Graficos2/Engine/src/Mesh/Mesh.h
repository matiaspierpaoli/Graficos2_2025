#pragma once
#include "Exports.h"
#include <vector>
#include "Camera/Camera.h"
#include "Material/Material.h"
#include "Light/Light.h"

class GraficosEngine_API Mesh {
protected:
    unsigned int* vBuffer;
    unsigned int* iBuffer;
    unsigned int modelId;
    Material material;

public:
    Mesh();
    virtual ~Mesh();

    unsigned int GetVertexBuffer() const { return *vBuffer; }
    unsigned int GetIndexBuffer() const { return *iBuffer; }
    unsigned int GetModelId() const { return modelId; }

    void SetMaterial(const Material& m) { material = m; }
    const Material& GetMaterial() const { return material; }

    void SetModelId(unsigned int id) { modelId = id; }

    virtual void Render(Camera* camera,
        const std::vector<DirectionalLight>& dirLights,
        const std::vector<PointLight>& pointLights,
        const std::vector<SpotLight>& spotLights) = 0;
};
