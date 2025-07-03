#pragma once
#include "Mesh/Mesh.h"
#include "Material/Material.h"
#include <vector>

class MeshIndexed : public Mesh {
    unsigned int vertexCount;
public:
    MeshIndexed() = default;
    ~MeshIndexed() override = default;

    void SetupBuffers(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);

    void Render(
        Camera* camera,
        const std::vector<DirectionalLight>& dirLights,
        const std::vector<PointLight>& pointLights,
        const std::vector<SpotLight>& spotLights
    ) override;
};
