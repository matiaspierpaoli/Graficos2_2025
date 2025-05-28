#include "Material/Material.h"
#include <RendererSingleton.h>
#include <string>

Material::Material(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shine, const std::string& texturePath)
    : ambient(amb), diffuse(diff), specular(spec), shininess(shine)
{
    if (!texturePath.empty()) {
        diffuseTexture = RendererSingleton::GetRenderer()->LoadTexture(texturePath);
        useTexture = true;
    }
}
