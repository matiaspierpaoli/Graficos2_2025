#include "CubeMesh.h"

CubeMesh::CubeMesh(Sprite* sprites[6]) {
    for (int i = 0; i < 6; ++i) {
        this->sprites[i] = sprites[i];
    }

    // Define cube faces
    cubeFacePositions[0] = glm::vec3(0.0f, 0.0f, 0.5f);  // Front
    cubeFacePositions[1] = glm::vec3(0.0f, 0.0f, -0.5f); // Back
    cubeFacePositions[2] = glm::vec3(-0.5f, 0.0f, 0.0f); // Left
    cubeFacePositions[3] = glm::vec3(0.5f, 0.0f, 0.0f);  // Right
    cubeFacePositions[4] = glm::vec3(0.0f, 0.5f, 0.0f);  // Above
    cubeFacePositions[5] = glm::vec3(0.0f, -0.5f, 0.0f); // Below

    // Define rotations
    cubeFaceRotations[0] = glm::quat();
    cubeFaceRotations[1] = glm::quat(glm::radians(glm::vec3(180.0f, 0.0f, 0.0f)));
    cubeFaceRotations[2] = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
    cubeFaceRotations[3] = glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));
    cubeFaceRotations[4] = glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f)));
    cubeFaceRotations[5] = glm::quat(glm::radians(glm::vec3(90.0f, 0.0f, 0.0f)));

    generateBuffers();
}

CubeMesh::~CubeMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void CubeMesh::generateBuffers() {
    // Cube vertices (each face is a square)
    GLfloat vertices[] = {
        // Position         // Texture Coord
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // Left down
         0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // Right down
         0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // Up right
        -0.5f,  0.5f,  0.0f,  0.0f, 1.0f  // Up left
    };

    // Indices for a square (2 triangles in a face)
    GLuint indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); 
}

void CubeMesh::Render() {

    for (int i = 0; i < 6; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubeFacePositions[i]);
        model = model * glm::mat4_cast(cubeFaceRotations[i]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
