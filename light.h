#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "lightVAO.h"

class Light {
private:
	std::vector<float> vertices = {};

	glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
		float x = r * sin(theta) * cos(phi);
		float y = r * cos(theta);
		float z = r * sin(theta) * sin(phi);
		return glm::vec3(x, y, z);
	};
public:
	glm::vec3 position = {};
	float radius = 0.125f;
	LightVAO Vao;

	Light(glm::vec3 pos, float rad) : position(pos), radius(rad) {
		int stacks = 10;
		int sectors = 10;

		for (float i = 0.0f; i <= stacks; ++i) {
			float theta1 = (i / stacks) * glm::pi<float>();
			float theta2 = (i + 1) / stacks * glm::pi<float>();
			for (float j = 0.0f; j < sectors; ++j) {
				float phi1 = j / sectors * 2 * glm::pi<float>();
				float phi2 = (j + 1) / sectors * 2 * glm::pi<float>();
				glm::vec3 v1 = sphericalToCartesian(radius, theta1, phi1);
				glm::vec3 v2 = sphericalToCartesian(radius, theta1, phi2);
				glm::vec3 v3 = sphericalToCartesian(radius, theta2, phi1);
				glm::vec3 v4 = sphericalToCartesian(radius, theta2, phi2);

				// Triangle 1: v1-v2-v3
				vertices.insert(vertices.end(), { v1.x, v1.y, v1.z }); //      /|
				vertices.insert(vertices.end(), { v2.x, v2.y, v2.z }); //     / |
				vertices.insert(vertices.end(), { v3.x, v3.y, v3.z }); //    /__|

				// Triangle 2: v2-v4-v3
				vertices.insert(vertices.end(), { v2.x, v2.y, v2.z });
				vertices.insert(vertices.end(), { v4.x, v4.y, v4.z });
				vertices.insert(vertices.end(), { v3.x, v3.y, v3.z });
			}
		}

		LightVAO vao(vertices);
		Vao = vao;
	}

	void draw() {
		Vao.bindVAO();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
	}
};