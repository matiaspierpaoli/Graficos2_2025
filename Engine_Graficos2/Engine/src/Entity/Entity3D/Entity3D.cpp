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

void Entity3D::SetMaterialToMeshes(bool recursive)
{
    for (auto* m : meshes)
        m->SetMaterial(material);

    if (recursive) {
        for (Entity* c : GetChildren()) {
            if (auto* e3d = dynamic_cast<Entity3D*>(c)) {
                e3d->SetMaterial(material);
                e3d->SetMaterialToMeshes(true);
            }
        }
    }
}

void Entity3D::RenderRecursive(Camera* camera, 
    const std::vector<DirectionalLight>& dirLights, 
    const std::vector<PointLight>& pointLights, 
    const std::vector<SpotLight>& spotLights)
{
    for (auto* m : meshes) {
        if (m->GetMaterial().diffuseTexture == 0 && material.useTexture)
            m->SetMaterial(material);
        m->Render(camera, dirLights, pointLights, spotLights);
    }

    for (Entity* c : GetChildren()) {
        if (auto* e3d = dynamic_cast<Entity3D*>(c)) {
            e3d->RenderRecursive(camera, dirLights, pointLights, spotLights);
        }
    }
}

void Entity3D::Render(
    Camera* camera,
    const std::vector<DirectionalLight>& dirLights,
    const std::vector<PointLight>& pointLights,
    const std::vector<SpotLight>& spotLights
) {
    RenderRecursive(camera, dirLights, pointLights, spotLights);
}
