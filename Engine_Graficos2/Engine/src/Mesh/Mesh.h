#pragma once
#include "Exports.h"
#include <../libs/glm/glm.hpp>
#include <vector>

class GraficosEngine_API Mesh {
protected:
    unsigned int VAO, VBO, EBO;
    unsigned int vertexCount;
    unsigned int shaderID;

    glm::mat4 modelMatrix = glm::mat4(1.0f);

public:
    Mesh();
    virtual ~Mesh();

    virtual void Draw(const glm::mat4& view, const glm::mat4& proj);

    void SetModelMatrix(const glm::mat4& model);
    glm::mat4 GetModelMatrix() const;

    void SetShader(unsigned int shader);
};
