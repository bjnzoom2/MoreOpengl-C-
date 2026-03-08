#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vao.h"

class Object {
private:
	std::vector<float> vertices;
public:
	glm::vec2 initPosition = {};
	glm::vec2 position = {};
	glm::vec2 velocity = {};
	float mass = 1;
	float radius = 0.125f;
	int res = 50;
	glm::vec2 totalForce = {};

	VAO Vao;

	Object(glm::vec2 initPos, glm::vec2 vel, float _mass, float rad) : initPosition(initPos), position(initPos), velocity(vel), mass(_mass), radius(rad) {
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		vertices.push_back(1.0f);
		vertices.push_back(1.0f);
		vertices.push_back(1.0f);
		for (int i = 0; i <= res; i++) {
			float angle = 2.0f * 3.14159265359 * (static_cast<float>(i) / res);
			float x = cos(angle) * radius;
			float y = sin(angle) * radius;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(0.0f);

			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
		}

		VAO vao(vertices);
		Vao = vao;
	}

	void draw() {
		Vao.bindVAO();
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 6);
	}

	void accelerate(float deltatime) {
		glm::vec2 accel = totalForce / mass;
		velocity += accel * deltatime;
		position += velocity * deltatime;
	}
};