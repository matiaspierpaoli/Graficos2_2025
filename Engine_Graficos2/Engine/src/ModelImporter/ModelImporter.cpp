#include "ModelImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "RendererSingleton.h"
#include "Material/Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Convert Assimp’s row-major matrix to GLM’s column-major format
static glm::mat4 AiToGlm(const aiMatrix4x4& a) {
    return glm::transpose(glm::make_mat4(&a.a1));
}

// Build a MeshIndexed from a single aiMesh, applying the given transform
static MeshIndexed* processMesh(aiMesh* mesh,
    const aiScene* scene,
    const std::string& dir,
    const glm::mat4& transform)
{
    // 1) Gather vertex data (position, normal, UV)
    std::vector<float> verts;
    verts.reserve(mesh->mNumVertices * 8);

    for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
        // apply node transform to vertex position
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

        // copy normal
        verts.push_back(mesh->mNormals[i].x);
        verts.push_back(mesh->mNormals[i].y);
        verts.push_back(mesh->mNormals[i].z);

        // copy UV if present, else zero
        if (mesh->mTextureCoords[0]) {
            verts.push_back(mesh->mTextureCoords[0][i].x);
            verts.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            verts.push_back(0.0f);
            verts.push_back(0.0f);
        }
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

    // 4) Default material (ambient, diffuse, specular, shininess, no texture)
    Material mat(
        glm::vec3(0.1f),
        glm::vec3(1.0f),
        glm::vec3(0.5f),
        32.0f,
        ""
    );

    // if the mesh has a material, try to load its diffuse/base_color texture
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
                texPath // set the texture path
            );
        }
    }

    m->SetMaterial(mat);
    return m;
}

// Recursively traverse scene graph, accumulating node transforms
static void processNode(aiNode* node,
    const aiScene* scene,
    const std::string& dir,
    std::vector<MeshIndexed*>& meshes,
    const glm::mat4& parentTransform)
{
    // combine parent transform with this node’s local transform
    glm::mat4 local = AiToGlm(node->mTransformation);
    glm::mat4 global = parentTransform * local;

    // process all meshes at this node
    for (unsigned i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* aiM = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(aiM, scene, dir, global));
    }

    // recurse into children
    for (unsigned i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, dir, meshes, global);
    }
}

// Public API: load all meshes from a model file
std::vector<MeshIndexed*> ModelImporter::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | // force triangles
        aiProcess_GenNormals | // generate normals if needed
        aiProcess_CalcTangentSpace
    );
    if (!scene || !scene->mRootNode) {
        // failed to load
        return {};
    }

    // extract directory to locate textures
    std::string dir = path.substr(0, path.find_last_of('/'));
    std::vector<MeshIndexed*> meshes;
    meshes.reserve(scene->mNumMeshes);

    // start recursion with identity transform
    processNode(scene->mRootNode, scene, dir, meshes, glm::mat4(1.0f));
    return meshes;
}
