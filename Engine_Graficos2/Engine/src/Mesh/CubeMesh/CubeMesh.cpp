#include "CubeMesh.h"
#include "Camera/Camera.h"
#include "../src/Window/Window.h"
#include "RendererSingleton.h"
#include <glm/gtc/type_ptr.hpp>


CubeMesh::CubeMesh(Sprite* faces[6])
{
    for (int i = 0; i < 6; ++i) {
        this->faces[i] = faces[i];
    }

    SetupMesh();
}

CubeMesh::~CubeMesh()
{
    // No eliminamos los sprites, solo los usamos como referencia
}

void CubeMesh::SetupMesh()
{
    // Definición de vértices (posición + coordenadas de textura)
    float vertices[] = {
        // Cara frontal (Z+) - Textura 0
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        // Cara posterior (Z-) - Textura 1
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

        // Cara derecha (X+) - Textura 2
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

         // Cara izquierda (X-) - Textura 3
         -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

         // Cara superior (Y+) - Textura 4
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,

         // Cara inferior (Y-) - Textura 5
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f
    };

    // Índices para renderizar el cubo (2 triángulos por cara)
    unsigned int indices[] = {
        // Cara frontal
        0, 1, 2,
        2, 3, 0,

        // Cara posterior
        4, 5, 6,
        6, 7, 4,

        // Cara derecha
        8, 9, 10,
        10, 11, 8,

        // Cara izquierda
        12, 13, 14,
        14, 15, 12,

        // Cara superior
        16, 17, 18,
        18, 19, 16,

        // Cara inferior
        20, 21, 22,
        22, 23, 20
    };

    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);*/

    *vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(vertices, sizeof(vertices), true);
    *iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(indices, 36);
}

void CubeMesh::Render(Camera* camera)
{
    // Obtener matrices de vista y proyección
    Window* window = RendererSingleton::GetRenderer()->GetWindow();
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj = camera->GetProjectionMatrix(window);

    //RendererSingleton::GetRenderer()->SetSprite(0);

    // Renderizar cada cara con su textura correspondiente
    for (unsigned int i = 0; i < 6; i++) {
        // Activar la textura del sprite correspondiente
        faces[i]->Bind(0);

        // Dibujar los triángulos de esta cara
        // Nota: Esto requiere que tu Renderer soporte dibujar por rangos de índices
        RendererSingleton::GetRenderer()->DrawRange(
            *vBuffer,
            *iBuffer,
            modelId,
            view,
            proj,
            i * 6, // offset inicial
            6      // count de índices
        );

        faces[i]->Unbind();
    }
}