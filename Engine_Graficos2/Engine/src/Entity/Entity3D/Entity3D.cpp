#include "Entity3D.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Entity3D::Entity3D()
    : mesh(nullptr)
{
}

Entity3D::~Entity3D()
{
    if (mesh) {
        delete mesh;
        mesh = nullptr;
    }
}

void Entity3D::SetMesh(Mesh* newMesh)
{
    mesh = newMesh;
    UpdateModel(false); 
}

Mesh* Entity3D::GetMesh()
{
    return mesh;
}

void Entity3D::UpdateModel(bool isModelCreated)
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scal = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x / 2, scale.y / 2, 1.0f));

    glm::mat4 model = trans * rotX * rotY * rotZ * scal;

    if (mesh)
        mesh->SetModelMatrix(model);
}

void Entity3D::Draw(const glm::mat4& view, const glm::mat4& proj)
{
    if (mesh)
        mesh->Draw(view, proj);
}
