#include "Mesh.h"
#include <../libs/glew/include/GL/glew.h> 
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), vertexCount(0), shaderID(0) {}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(const glm::mat4& view, const glm::mat4& proj) {
    if (shaderID == 0) return;

    glUseProgram(shaderID);

    glm::mat4 mvp = proj * view * modelMatrix;
    int mvpLoc = glGetUniformLocation(shaderID, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::SetModelMatrix(const glm::mat4& model) {
    modelMatrix = model;
}

glm::mat4 Mesh::GetModelMatrix() const {
    return modelMatrix;
}

void Mesh::SetShader(unsigned int shader) {
    shaderID = shader;
}
