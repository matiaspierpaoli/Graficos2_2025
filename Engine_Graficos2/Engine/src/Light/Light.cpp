#include "Light/Light.h"
#include "RendererSingleton.h"

DirectionalLight::DirectionalLight(const glm::vec3& dir): Light(), direction(dir)
{
}

PointLight::PointLight(const glm::vec3& pos) : Light(), position(pos), constant(1.0f), linear(0.09f), quadratic(0.032f)
{
}

SpotLight::SpotLight(const glm::vec3& pos, const glm::vec3& dir) : Light(), position(pos), direction(dir), cutOff(glm::cos(glm::radians(12.5f))), outerCutOff(glm::cos(glm::radians(17.5f)))
{
}
