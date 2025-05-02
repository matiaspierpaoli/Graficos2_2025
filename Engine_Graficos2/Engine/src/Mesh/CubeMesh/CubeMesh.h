#pragma once
#include "../src/Mesh/Mesh.h"
#include "../src/Sprite/Sprite.h" 
#include "Exports.h"
#include "../libs/glew/include/GL/glew.h"
#include "../libs/glm/gtc/quaternion.hpp"
#include "../libs/glfw/include/GLFW/glfw3.h"

class GraficosEngine_API CubeMesh : public Mesh {
private:
    Sprite* sprites[6];    
    glm::vec3 cubeFacePositions[6];   
    glm::quat cubeFaceRotations[6];   

    GLuint VAO, VBO, EBO;

public:
    CubeMesh(Sprite* sprites[6]);
    ~CubeMesh();

    void generateBuffers();
    void Render();
};
