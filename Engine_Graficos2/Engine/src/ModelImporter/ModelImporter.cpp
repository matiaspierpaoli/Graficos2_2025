#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "RendererSingleton.h"
#include "Material/Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Convierte aiMatrix4x4 a glm::mat4
static glm::mat4 AiToGlm(const aiMatrix4x4& a) {
    return glm::transpose(glm::make_mat4(&a.a1));
}

// Procesa un único aiMesh con la transform acumulada
static MeshIndexed* processMesh(aiMesh* mesh,
    const aiScene* scene,
    const std::string& dir,
    const glm::mat4& transform)
{
    // 1) Empaquetar vértices transformados
    std::vector<float> verts;
    verts.reserve(mesh->mNumVertices * 8);

    for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
        // posición local -> world
        glm::vec4 p{
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z,
            1.0f
        };
        glm::vec4 pw = transform * p;
        verts.push_back(pw.x);
        verts.push_back(pw.y);
        verts.push_back(pw.z);

        // normales (ignoramos rot-escalado no uniformes; si te interesa, usa la inversa transpuesta)
        verts.push_back(mesh->mNormals[i].x);
        verts.push_back(mesh->mNormals[i].y);
        verts.push_back(mesh->mNormals[i].z);

        // UV
        if (mesh->mTextureCoords[0]) {
            verts.push_back(mesh->mTextureCoords[0][i].x);
            verts.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            verts.push_back(0.0f);
            verts.push_back(0.0f);
        }
    }

    // 2) Índices
    std::vector<unsigned int> idxs;
    idxs.reserve(mesh->mNumFaces * 3);
    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
            idxs.push_back(mesh->mFaces[i].mIndices[j]);
    }

    // 3) Crear MeshIndexed
    MeshIndexed* m = new MeshIndexed();
    m->SetupBuffers(verts, idxs);

    // 4) Material: intentamos cargar diffuse map
    Material mat(
        glm::vec3(0.1f),
        glm::vec3(1.0f),
        glm::vec3(0.5f),
        32.0f,
        ""
    );
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
        if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString str;
            aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string texPath = dir + "/" + str.C_Str();
            mat = Material(
                glm::vec3(0.1f),
                glm::vec3(1.0f),
                glm::vec3(0.5f),
                32.0f,
                texPath
            );
        }
    }

    m->SetMaterial(mat);
    return m;
}

// Recorre nodos recursivamente
static void processNode(aiNode* node,
    const aiScene* scene,
    const std::string& dir,
    std::vector<MeshIndexed*>& meshes,
    const glm::mat4& parentTransform)
{
    // 1) Calcula la transform global de este nodo
    glm::mat4 local = AiToGlm(node->mTransformation);
    glm::mat4 global = parentTransform * local;

    // 2) Procesa cada mesh de este nodo
    for (unsigned i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* aiM = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(aiM, scene, dir, global));
    }

    // 3) Recurse a los hijos
    for (unsigned i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, dir, meshes, global);
    }
}

std::vector<MeshIndexed*> ModelImporter::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace
    );
    if (!scene || !scene->mRootNode) {
        return {};
    }

    std::string dir = path.substr(0, path.find_last_of('/'));
    std::vector<MeshIndexed*> meshes;
    meshes.reserve(scene->mNumMeshes);

    // Llamada inicial con identidad
    processNode(scene->mRootNode, scene, dir, meshes, glm::mat4(1.0f));
    return meshes;
}
