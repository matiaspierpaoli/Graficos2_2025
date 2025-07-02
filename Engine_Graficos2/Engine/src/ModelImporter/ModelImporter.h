#pragma once

#include <string>
#include <vector>
#include "Mesh/IndexedMesh/MeshIndexed.h"

class GraficosEngine_API ModelImporter {
public:
    static std::vector<MeshIndexed*> LoadModel(const std::string& path);
};
