#ifndef table_chair_h
#define table_chair_h

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class Table_Chair {

public:
	std::vector<glm::mat4> modelMatrices;
	float tox, toy, toz;
	Table_Chair(float x = 0, float y = 0, float z = 0) {
		tox = x;
		toy = y;
		toz = z;
	}
	glm::mat4 transforamtion(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
		tx += tox;
		ty += toy;
		tz += toz;
		glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
		translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
		rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
		scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
		model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
		return model;
	}


	Shader ret_shader(Shader ourShader, unsigned int VAO, unsigned int VAO2, unsigned int VAO3, unsigned int VAO4, unsigned int VAO5) {
		glm::mat4 model;
		float rotateAngle_X = 0;
		float rotateAngle_Y = 0;
		float rotateAngle_Z = 0;
		//table surface
		model = transforamtion(0.5, 0, 2.5, rotateAngle_X, rotateAngle_Y+90, rotateAngle_Z, 7, 0.2, 3);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//left top Leg
		model = transforamtion(0.6, 0, -0.8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2, -1.5, 0.2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//right top Leg
		model = transforamtion(1.8, 0, -0.8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2, -1.5, 0.2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//right bottom Leg
		model = transforamtion(0.6, 0, 2.3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2, -1.5, 0.2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//left bottom Leg
		model = transforamtion(1.8, 0, 2.3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2, -1.5, 0.2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		// Chair seat
		model = transforamtion(01, -.3, 2.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, -1, 1); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Chair back
		model = transforamtion(01, 0.37, 2.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, -1.35, 0.2); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (left side)
		model = transforamtion(1, -.8, 2.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (right side)
		model = transforamtion(1.51, -.8, 2.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



		//2nd chair
		// Chair seat
		model = transforamtion(01, -.3, -1.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, -1, 1); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Chair back
		model = transforamtion(01, 0.37, -1.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, -1.35, 0.2); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (left side)
		model = transforamtion(1, -.8, -1.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (right side)
		model = transforamtion(1.51, -.8, -1.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//3rd chair
		// Chair seat
		model = transforamtion(-.2, -.3, 1, rotateAngle_X, rotateAngle_Y + 90, rotateAngle_Z, 1, -1, 1); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Chair back
		model = transforamtion(-.2, 0.37, 1, rotateAngle_X, rotateAngle_Y+90, rotateAngle_Z, 1, -1.35, 0.2); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (left side)
		model = transforamtion(-.2, -.8, 1, rotateAngle_X, rotateAngle_Y + 90, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (right side)
		model = transforamtion(-0.2, -.8, 0.49, rotateAngle_X, rotateAngle_Y + 90, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//4th chair
		// Chair seat
		model = transforamtion(2.5, -.3, .5, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, 1, -1, 1); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Chair back
		model = transforamtion(2.5, 0.37, .5, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, 1, -1.35, 0.2); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (left side)
		model = transforamtion(2.5, -.8, .5, rotateAngle_X, rotateAngle_Y - 90, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// Far seat holder (right side)
		model = transforamtion(2.51, -.8, 1.01, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, -.01, 2.23, 0.13); // Moved slightly right
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		return ourShader;
	} 
};


#endif