#include "Entity3D.h"
#include "RendererSingleton.h"

Entity3D::Entity3D() : Entity() {}

Entity3D::~Entity3D() {
    for (auto* m : meshes)
        delete m;
    meshes.clear();
}

void Entity3D::AddMesh(Mesh* m) {
    meshes.push_back(m);
    unsigned int id = RendererSingleton::GetRenderer()
        ->GetNewModelId(GetTransformMatrix());
    m->SetModelId(id);
}

void Entity3D::UpdateModel(bool isModelCreated) {
    Entity::UpdateModel(isModelCreated);
    auto& R = *RendererSingleton::GetRenderer();
    glm::mat4 tm = GetTransformMatrix();
    for (auto* m : meshes) {
        R.SetModel(tm, m->GetModelId());
    }
}

void Entity3D::SetMaterialToMeshes()
{
    for (auto* m : meshes) {
        m->SetMaterial(material);
    }
}

void Entity3D::Render(
    Camera* camera,
    const std::vector<DirectionalLight>& dirLights,
    const std::vector<PointLight>& pointLights,
    const std::vector<SpotLight>& spotLights
) {
    for (auto* m : meshes) {
        m->Render(camera, dirLights, pointLights, spotLights);
    }
}
