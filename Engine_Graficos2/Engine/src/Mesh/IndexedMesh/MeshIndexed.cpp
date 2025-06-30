#include "MeshIndexed.h"
#include "RendererSingleton.h"

void MeshIndexed::SetupBuffers(const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices) {

    *vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(
        vertices.data(),
        vertices.size() * sizeof(float),
        true
    );

    *iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(
        const_cast<unsigned int*>(indices.data()),
        indices.size()
    );

    vertexCount = indices.size();
}

void MeshIndexed::Render(Camera* camera, Material material,
    std::vector<DirectionalLight> dirLights,
    std::vector<PointLight> pointLights,
    std::vector<SpotLight> spotLights) {
    
    Renderer* renderer = RendererSingleton::GetRenderer();
    renderer->SetLightingShaderActive();

    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj = camera->GetProjectionMatrix(renderer->GetWindow());

    renderer->DrawWithLighting(
        *vBuffer,
        *iBuffer,
        modelId,
        view,
        proj,
        material,
        camera->GetPosition(),
        dirLights,
        pointLights,
        spotLights
    );
}