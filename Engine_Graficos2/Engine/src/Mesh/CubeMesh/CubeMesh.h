#pragma once
#include "../src/Mesh/Mesh.h"
#include "../src/Sprite/Sprite.h" 
#include "Exports.h"
#include "../libs/glew/include/GL/glew.h"
#include "../libs/glm/gtc/quaternion.hpp"
#include "../libs/glfw/include/GLFW/glfw3.h"

class GraficosEngine_API CubeMesh : public Mesh
{
public:
    CubeMesh();
    CubeMesh(Sprite* faces[6]);
    ~CubeMesh();

    void Render(Camera* camera,
        const std::vector<DirectionalLight>& dirLights,
        const std::vector<PointLight>& pointLights,
        const std::vector<SpotLight>& spotLights) override;

private:
    void SetupMesh();
    Sprite* faces[6];
};
