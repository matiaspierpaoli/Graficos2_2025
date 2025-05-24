// Material.h
#pragma once
#include <../libs/glm/glm.hpp>
#include "Exports.h"

class GraficosEngine_API Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(
        const glm::vec3& amb = glm::vec3(0.1f),
        const glm::vec3& diff = glm::vec3(1.0f),
        const glm::vec3& spec = glm::vec3(0.5f),
        float shine = 32.0f
    ) : ambient(amb), diffuse(diff), specular(spec), shininess(shine) {
    }
};