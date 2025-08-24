#pragma once

#include <string>
#include <vector>
#include "Entity/Entity3D/Entity3D.h"
#include "Mesh/IndexedMesh/MeshIndexed.h"

// Define assimp copy struct to avoid the assimp include in .h
struct aiNode;
struct aiScene;
struct aiMesh;

class GraficosEngine_API ModelImporter {
public:
    static Entity3D* LoadModelWithHierarchy(const std::string& path);
private:
    static Entity3D* ProcessNode(aiNode* node, const aiScene* scene,
        const std::string& dir, Entity3D* parent);
    static MeshIndexed* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir);
    static void DecomposeMatrix(const glm::mat4& matrix, Vector3& translation,
        Vector3& rotation, Vector3& scale);
};
