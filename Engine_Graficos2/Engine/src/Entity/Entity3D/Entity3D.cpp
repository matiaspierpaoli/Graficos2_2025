#include "Entity3D.h"
#include "RendererSingleton.h"

Entity3D::Entity3D() : mesh(nullptr)
{
}

Entity3D::~Entity3D()
{
    if (mesh != nullptr) {
        delete mesh;
    }
}

void Entity3D::SetMesh(Mesh* mesh)
{
    if (this->mesh != nullptr) 
    {
        delete this->mesh; // Limpiar el mesh anterior si existe
    }
    this->mesh = mesh;

    //Sincronizar el modelId
    if (mesh != nullptr) 
    {
        mesh->SetModelId(this->modelId);
    }
}

Mesh* Entity3D::GetMesh() const
{
    return mesh;
}

void Entity3D::UpdateModel(bool isModelCreated)
{
    Entity::UpdateModel(isModelCreated);

    if (mesh != nullptr) {
        // Sincroniza la matriz de transformación con el mesh
        RendererSingleton::GetRenderer()->SetModel(GetTransformMatrix(), modelId);
    }
}
