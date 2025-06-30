#pragma once
#include "Mesh/Mesh.h"

class GraficosEngine_API MeshIndexed : public Mesh {
public:
    void SetupBuffers(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);
    void Render(Camera* camera, Material material,
        std::vector<DirectionalLight> dirLights,
        std::vector<PointLight> pointLights,
        std::vector<SpotLight> spotLights) override;
private:
    unsigned int vertexCount;
};