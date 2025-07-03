#include "Mesh.h"
#include "RendererSingleton.h"

Mesh::Mesh()
{
    vBuffer = new unsigned int(0);
    iBuffer = new unsigned int(0);
    modelId = 0; 
}

Mesh::~Mesh()
{
    delete vBuffer;
    delete iBuffer;
}
