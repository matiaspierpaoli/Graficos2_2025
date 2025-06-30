#include "ModelImporter.h"
#include "RendererSingleton.h"
#include <Mesh/IndexedMesh/MeshIndexed.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh* ModelImporter::LoadMesh(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return nullptr;
    }

    // Process first model mesh
    aiMesh* aiMesh = scene->mMeshes[0];
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Extract vertices
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        // Posición
        vertices.push_back(aiMesh->mVertices[i].x);
        vertices.push_back(aiMesh->mVertices[i].y);
        vertices.push_back(aiMesh->mVertices[i].z);

        // Normal
        vertices.push_back(aiMesh->mNormals[i].x);
        vertices.push_back(aiMesh->mNormals[i].y);
        vertices.push_back(aiMesh->mNormals[i].z);

        // UV (si existe)
        if (aiMesh->mTextureCoords[0]) {
            vertices.push_back(aiMesh->mTextureCoords[0][i].x);
            vertices.push_back(aiMesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    // Extract indexes
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace face = aiMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    MeshIndexed* mesh = new MeshIndexed();
    mesh->SetupBuffers(vertices, indices);
    return mesh;
}