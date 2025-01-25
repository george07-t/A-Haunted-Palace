#ifndef SPHERE_H
#define SPHERE_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void drawSphere(Shader& shader, float radius, unsigned int segmentsU, unsigned int segmentsV, glm::vec3 color, float tx, float ty, float tz) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i <= segmentsU; ++i) {
        float theta = (float)i / (float)segmentsU * glm::pi<float>();
        for (unsigned int j = 0; j <= segmentsV; ++j) {
            float phi = (float)j / (float)segmentsV * 2.0f * glm::pi<float>();

            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);

            // Position
            vertices.push_back(x);  // X
            vertices.push_back(y);  // Y
            vertices.push_back(z);  // Z

            // Normal (normalized position on the sphere's surface)
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertices.push_back(normal.x);  // Normal X
            vertices.push_back(normal.y);  // Normal Y
            vertices.push_back(normal.z);  // Normal Z

            // Color
            vertices.push_back(color.r); // R
            vertices.push_back(color.g); // G
            vertices.push_back(color.b); // B
        }
    }

    for (unsigned int i = 0; i < segmentsU; ++i) {
        for (unsigned int j = 0; j < segmentsV; ++j) {
            unsigned int first = i * (segmentsV + 1) + j;
            unsigned int second = first + segmentsV + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

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

    // Draw the sphere
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

#endif
