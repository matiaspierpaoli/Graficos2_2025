#pragma once
#include <../libs/glm/glm.hpp>
#include "Exports.h"

constexpr int MAX_POINT_LIGHTS = 4;
constexpr int MAX_SPOT_LIGHTS = 2;

class Light {
public:
    glm::vec3 color;
    float intensity;

    Light(const glm::vec3& col = glm::vec3(1.0f), float intens = 1.0f)
        : color(col), intensity(intens) {
    }
    virtual ~Light() = default;
};

class GraficosEngine_API DirectionalLight : public Light {
public:
    glm::vec3 direction;

    DirectionalLight(const glm::vec3& dir = glm::vec3(-0.2f, -1.0f, -0.3f));
};

class GraficosEngine_API PointLight : public Light {
public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    PointLight(const glm::vec3& pos = glm::vec3(0.0f));
};

class GraficosEngine_API SpotLight : public Light {
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff; // Internal angle
    float outerCutOff; // External angle

    float constant;
    float linear;
    float quadratic;

    SpotLight(const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& dir = glm::vec3(0.0f, 0.0f, -1.0f), float cut = 12.5f, float outerCut = 17.5f);
};