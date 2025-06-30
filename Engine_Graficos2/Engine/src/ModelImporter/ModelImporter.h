#pragma once
#include "Mesh/Mesh.h"

class GraficosEngine_API ModelImporter {
public:
    static Mesh* LoadMesh(const std::string& path);
};