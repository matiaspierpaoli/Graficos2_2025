#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "RendererSingleton.h"
#include "Material/Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static glm::mat4 AiToGlm(const aiMatrix4x4& a) {
    return glm::transpose(glm::make_mat4(&a.a1));
}

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

        // normales (ignoramos escala no uniforme aquí)
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

        if (i == 0) {
            std::cout << "UV0[0]=("
                << mesh->mTextureCoords[0][0].x << ","
                << mesh->mTextureCoords[0][0].y << ")\n";
        }
        // y si tienes channel 1:
        if (mesh->mNumUVComponents[1] && i == 0) {
            std::cout << "UV1[0]=("
                << mesh->mTextureCoords[1][0].x << ","
                << mesh->mTextureCoords[1][0].y << ")\n";
        }
    }

    // 2) Índices
    std::vector<unsigned int> idxs;
    idxs.reserve(mesh->mNumFaces * 3);
    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
            idxs.push_back(mesh->mFaces[i].mIndices[j]);
    }

    // 3) Crear MeshIndexed y sus buffers
    MeshIndexed* m = new MeshIndexed();
    m->SetupBuffers(verts, idxs);

    // 4) Material: detectamos el canal de Albedo correcto
    Material mat(
        glm::vec3(0.1f),   // ambient
        glm::vec3(1.0f),   // diffuse color base
        glm::vec3(0.5f),   // specular
        32.0f,             // shininess
        ""                 // ruta a textura (vacío = placeholder blanco)
    );

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

        // elegimos primero DIFFUSE, si no existe usamos BASE_COLOR
        aiTextureType type = aiTextureType_DIFFUSE;
        if (aiMat->GetTextureCount(type) == 0
            && aiMat->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
        {
            type = aiTextureType_BASE_COLOR;
        }

        // si hay textura en ese canal, la cargamos
        if (aiMat->GetTextureCount(type) > 0) {
            aiString str;
            aiMat->GetTexture(type, 0, &str);
            std::string texPath = dir + "/" + str.C_Str();
            mat = Material(
                glm::vec3(0.1f),
                glm::vec3(1.0f),
                glm::vec3(0.5f),
                32.0f,
                texPath      // aquí la ruta real al albedo
            );
        }

        aiString str;
        unsigned int cntD = aiMat->GetTextureCount(aiTextureType_DIFFUSE);
        unsigned int cntB = aiMat->GetTextureCount(aiTextureType_BASE_COLOR);
        std::cout << "Mesh «" << mesh->mName.C_Str()
            << "» matIndex=" << mesh->mMaterialIndex
            << " diffuse=" << cntD
            << " baseColor=" << cntB;

        aiTextureType pick = (cntD > 0 ? aiTextureType_DIFFUSE : aiTextureType_BASE_COLOR);
        if (aiMat->GetTextureCount(pick) > 0) {
            aiMat->GetTexture(pick, 0, &str);
            std::cout << " -> cargando «" << str.C_Str() << "»\n";
        }
        else {
            std::cout << " -> sin textura, usar placeholder\n";
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
        /*aiProcess_FlipUVs |*/
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
