#include "Mesh.h"
#include "RendererSingleton.h"

Mesh::Mesh()
{
    vBuffer = new unsigned int(0);
    iBuffer = new unsigned int(0);
    modelId = 0; // Se establecerá más tarde por la Entity
}

Mesh::~Mesh()
{
    // No eliminamos modelId porque es manejado por Entity
    delete vBuffer;
    delete iBuffer;
}
