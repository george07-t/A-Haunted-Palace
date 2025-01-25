#ifndef CONE_H
#define CONE_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void drawCone(Shader& shader, float baseRadius, float height, unsigned int segments, glm::vec3 color, float tx, float ty, float tz) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Cone tip (center of the top of the cone)
    glm::vec3 tip = glm::vec3(0.0f, height, 0.0f);
    vertices.push_back(tip.x);  // X
    vertices.push_back(tip.y);  // Y
    vertices.push_back(tip.z);  // Z
    glm::vec3 tipNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    vertices.push_back(tipNormal.x);  // Normal X
    vertices.push_back(tipNormal.y);  // Normal Y
    vertices.push_back(tipNormal.z);  // Normal Z
    vertices.push_back(color.r);  // R
    vertices.push_back(color.g);  // G
    vertices.push_back(color.b);  // B

    // Generate the base vertices around the circle
    std::vector<glm::vec3> baseVertices;
    for (unsigned int i = 0; i <= segments; ++i) {
        float angle = (float)i / (float)segments * 2.0f * glm::pi<float>();
        float x = baseRadius * cos(angle);
        float z = baseRadius * sin(angle);

        // Store the base vertices (we use this for indices later)
        baseVertices.push_back(glm::vec3(x, 0.0f, z));

        // Base vertices (flat, facing downwards, normal pointing down)
        vertices.push_back(x);  // X
        vertices.push_back(0.0f); // Y
        vertices.push_back(z);  // Z
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z)); // Normal for the base (pointing outwards)
        vertices.push_back(normal.x);  // Normal X
        vertices.push_back(normal.y);  // Normal Y
        vertices.push_back(normal.z);  // Normal Z
        vertices.push_back(color.r);  // R
        vertices.push_back(color.g);  // G
        vertices.push_back(color.b);  // B
    }

    // Generate indices for the cone's sides (tip to base vertices)
    for (unsigned int i = 0; i < segments; ++i) {
        indices.push_back(0);  // Tip of the cone
        indices.push_back(i + 1);  // Base vertex i
        indices.push_back(i + 2);  // Base vertex i+1
    }

    // Add the last base vertex to form a closed loop
    indices.push_back(0);  // Tip of the cone
    indices.push_back(segments);  // Last base vertex
    indices.push_back(1);  // First base vertex

    // Create VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))); // Color
    glEnableVertexAttribArray(2);

    // Apply the translation transformation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tx, ty, tz));  // Translate by (tx, ty, tz)

    // Pass the transformation to the shader
    shader.setMat4("model", model);

    // Set material properties using the passed color
    shader.use();
    shader.setVec3("material.ambient", color * 0.2f); // Ambient color
    shader.setVec3("material.diffuse", color * 0.8f); // Diffuse color
    shader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // Specular color
    shader.setFloat("material.shininess", 32.0f); // Shininess

    // Draw the cone
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

#endif
