#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "RendererSingleton.h"
#include "Material/Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

// Convert Assimp’s row-major matrix to GLM’s column-major format
static glm::mat4 AiToGlm(const aiMatrix4x4& a) {
    return glm::transpose(glm::make_mat4(&a.a1));
}

void ModelImporter::DecomposeMatrix(const glm::mat4& matrix, Vector3& translation,
    Vector3& rotation, Vector3& scale) {
    // Usar glm::decompose para extraer transformaciones
    glm::vec3 glmTranslation, glmScale, skew;
    glm::quat orientation;
    glm::vec4 perspective;

    if (glm::decompose(matrix, glmScale, orientation, glmTranslation, skew, perspective)) {
        translation.x = glmTranslation.x;
        translation.y = glmTranslation.y;
        translation.z = glmTranslation.z;

        scale.x = glmScale.x;
        scale.y = glmScale.y;
        scale.z = glmScale.z;

        // Convertir quaternion a ángulos de Euler (en grados)
        glm::vec3 euler = glm::eulerAngles(orientation) * 180.0f / glm::pi<float>();
        rotation.x = euler.x;
        rotation.y = euler.y;
        rotation.z = euler.z;
    }
    else {
        // Si falla, valores por defecto
        translation = { 0,0,0 };
        rotation = { 0,0,0 };
        scale = { 1,1,1 };
    }
}

// Build a MeshIndexed from a single aiMesh, applying the given transform
MeshIndexed* ModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir) {
    // 1) Gather vertex data (position, normal, UV)
    std::vector<float> verts;
    verts.reserve(mesh->mNumVertices * 8);

    AABB bbox;
    for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
        // position
        verts.push_back(mesh->mVertices[i].x);
        verts.push_back(mesh->mVertices[i].y);
        verts.push_back(mesh->mVertices[i].z);

        // normal
        verts.push_back(mesh->mNormals[i].x);
        verts.push_back(mesh->mNormals[i].y);
        verts.push_back(mesh->mNormals[i].z);

        // UV if present, else zero
        if (mesh->mTextureCoords[0]) {
            verts.push_back(mesh->mTextureCoords[0][i].x);
            verts.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            verts.push_back(0.0f);
            verts.push_back(0.0f);
        }

        glm::vec3 p(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        bbox.expand(p);
    }

    // 2) Build index list from faces
    std::vector<unsigned int> idxs;
    idxs.reserve(mesh->mNumFaces * 3);
    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
            idxs.push_back(mesh->mFaces[i].mIndices[j]);
    }

    // 3) Create the GPU buffers
    MeshIndexed* m = new MeshIndexed();
    m->SetupBuffers(verts, idxs);
    m->SetLocalAABB(bbox);

    // 4) Material
    Material mat(
        glm::vec3(0.1f),
        glm::vec3(1.0f),
        glm::vec3(0.5f),
        32.0f,
        ""
    );

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

        // prefer DIFFUSE, fallback to BASE_COLOR
        aiTextureType type = aiTextureType_DIFFUSE;
        if (aiMat->GetTextureCount(type) == 0
            && aiMat->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
        {
            type = aiTextureType_BASE_COLOR;
        }

        if (aiMat->GetTextureCount(type) > 0) {
            aiString str;
            aiMat->GetTexture(type, 0, &str);
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

Entity3D* ModelImporter::ProcessNode(aiNode* node, const aiScene* scene,
    const std::string& dir, Entity3D* parent)
{
    Entity3D* entity = new Entity3D();
    entity->SetName(node->mName.C_Str());

    glm::mat4 glmMat = AiToGlm(node->mTransformation);

    Vector3 t, r, s;
    DecomposeMatrix(glmMat, t, r, s);

    entity->Translate(t.x, t.y, t.z);
    entity->RotateX(r.x);
    entity->RotateY(r.y);
    entity->RotateZ(r.z);
    entity->Scale(s.x, s.y, s.z);

    for (unsigned i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        MeshIndexed* m = ProcessMesh(aimesh, scene, dir);
        entity->AddMesh(m);
    }

    if (parent) parent->AddChild(entity);

    for (unsigned i = 0; i < node->mNumChildren; ++i)
        ProcessNode(node->mChildren[i], scene, dir, entity);

    return entity;
}

Entity3D* ModelImporter::LoadModelWithHierarchy(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | // force triangles
        aiProcess_GenNormals | // generate normals if needed
        aiProcess_CalcTangentSpace
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    // extract directory to locate textures
    std::string dir = path.substr(0, path.find_last_of('/'));

    // start recursion with identity transform
    Entity3D* root = ProcessNode(scene->mRootNode, scene, dir, nullptr);
    return root;
}
