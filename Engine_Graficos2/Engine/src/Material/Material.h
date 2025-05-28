// Material.h
#pragma once
#include <../libs/glm/glm.hpp>
#include "Exports.h"
#include <string>

class GraficosEngine_API Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    unsigned int diffuseTexture = 0; // 0 = sin textura  
    bool useTexture = false;

    Material(
        const glm::vec3& amb = glm::vec3(0.1f),
        const glm::vec3& diff = glm::vec3(1.0f),
        const glm::vec3& spec = glm::vec3(0.5f),
        float shine = 32.0f,
        const std::string& texturePath = ""
    );
};