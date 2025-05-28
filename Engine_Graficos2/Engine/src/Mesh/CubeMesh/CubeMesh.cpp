#include "CubeMesh.h"
#include "Camera/Camera.h"
#include "../src/Window/Window.h"
#include "RendererSingleton.h"
#include "Light/Light.h"
#include <glm/gtc/type_ptr.hpp>


CubeMesh::CubeMesh()
{
    SetupMesh();
}

CubeMesh::CubeMesh(Sprite* faces[6])
{
    for (int i = 0; i < 6; ++i) {
        this->faces[i] = faces[i];
    }

    SetupMesh();
}

CubeMesh::~CubeMesh()
{

}

void CubeMesh::SetupMesh()
{
    // Vertex format: 3 position + 3 normal = 6 floats
    float vertices[] = {
        // Front face (Z+)
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // LB
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // RB
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // RT
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // LT

        // Back face (Z-)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

        // Right face (X+)
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

         // Left face (X-)
         -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

         // Top face (Y+)
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

         // Bottom face (Y-)
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,       // Front
        4, 5, 6,  6, 7, 4,       // Back
        8, 9,10, 10,11, 8,       // Right
       12,13,14, 14,15,12,       // Left
       16,17,18, 18,19,16,       // Up
       20,21,22, 22,23,20        // Down
    };

    // Crear buffers
    *vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(vertices, sizeof(vertices), true);
    *iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(indices, 36);
}

void CubeMesh::Render(Camera* camera, Material cubeMaterial, std::vector<DirectionalLight> activeDirLights, std::vector<PointLight> activePointLights, std::vector<SpotLight> activeSpotLights) {
    Renderer* renderer = RendererSingleton::GetRenderer();
    renderer->SetLightingShaderActive();

    // Obtener matrices de vista y proyección
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj = camera->GetProjectionMatrix(renderer->GetWindow());

    // Dibujar con el nuevo método
    renderer->DrawWithLighting(
        *vBuffer,
        *iBuffer,
        modelId,
        view,
        proj,
        cubeMaterial,
        camera->GetPosition(),
        activeDirLights,
        activePointLights,
        activeSpotLights
    );
}