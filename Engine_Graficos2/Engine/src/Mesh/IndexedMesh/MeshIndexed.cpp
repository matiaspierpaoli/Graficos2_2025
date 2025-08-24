#include "MeshIndexed.h"
#include "RendererSingleton.h"

void MeshIndexed::SetupBuffers(
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices
) {
    auto R = RendererSingleton::GetRenderer();
    *vBuffer = R->GetNewVertexBuffer(vertices.data(),
        vertices.size() * sizeof(float),
        true);
    *iBuffer = R->GetNewIndexBuffer(
        const_cast<unsigned int*>(indices.data()),
        indices.size());
    vertexCount = indices.size();
}

void MeshIndexed::Render(
    Camera* camera,
    const std::vector<DirectionalLight>& dirLights,
    const std::vector<PointLight>& pointLights,
    const std::vector<SpotLight>& spotLights
) {
    auto R = RendererSingleton::GetRenderer();
    R->SetLightingShaderActive();

    R->DrawWithLighting(
        *vBuffer,
        *iBuffer,
        GetModelId(),
        camera->GetViewMatrix(),
        camera->GetProjectionMatrix(R->GetWindow()),
        material,               
        camera->GetPosition(),
        dirLights,
        pointLights,
        spotLights
    );
}

void MeshIndexed::SetLocalAABB(const AABB& b)
{
    localAABB = b;
}

const AABB& MeshIndexed::GetLocalAABB() const
{
    return localAABB;
}
