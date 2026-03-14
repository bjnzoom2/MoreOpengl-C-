#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vao.h"

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Material(glm::vec3 amb = glm::vec3(1.0f), glm::vec3 dif = glm::vec3(1.0f), glm::vec3 spec = glm::vec3(1.0f), float shine = 0.0f) : ambient(amb), diffuse(dif), specular(spec), shininess(shine) {}
};

class Object {
private:
	std::vector<float> vertices;

	glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
		float x = r * sin(theta) * cos(phi);
		float y = r * cos(theta);
		float z = r * sin(theta) * sin(phi);
		return glm::vec3(x, y, z);
	};
public:
	glm::vec3 position = {};
	glm::vec3 velocity = {};
	float mass = 1;
	float radius = 0.125f;
	glm::vec3 totalForce = {};
	Material material;

	bool isLight = false;

	VAO Vao;

	Object(glm::vec3 initPos, glm::vec3 vel, float _mass, float rad, Material mat, bool light) : position(initPos), velocity(vel), mass(_mass), radius(rad), material(mat), isLight(light) {
		int stacks = 50;
		int sectors = 50;

		// generate circumference points using integer steps
		for (float i = 0.0f; i <= stacks; ++i) {
			float theta1 = (i / stacks) * glm::pi<float>();
			float theta2 = (i + 1) / stacks * glm::pi<float>();
			for (float j = 0.0f; j < sectors; ++j) {
				float phi1 = j / sectors * 2 * glm::pi<float>();
				float phi2 = (j + 1) / sectors * 2 * glm::pi<float>();
				glm::vec3 v1 = sphericalToCartesian(this->radius, theta1, phi1);
				glm::vec3 v2 = sphericalToCartesian(this->radius, theta1, phi2);
				glm::vec3 v3 = sphericalToCartesian(this->radius, theta2, phi1);
				glm::vec3 v4 = sphericalToCartesian(this->radius, theta2, phi2);

				glm::vec3 n1 = glm::normalize(v1);
				glm::vec3 n2 = glm::normalize(v2);
				glm::vec3 n3 = glm::normalize(v3);
				glm::vec3 n4 = glm::normalize(v4);

				// Triangle 1: v1-v2-v3
				vertices.insert(vertices.end(), { v1.x, v1.y, v1.z, n1.x, n1.y, n1.z }); //      /|
				vertices.insert(vertices.end(), { v2.x, v2.y, v2.z, n2.x, n2.y, n2.z }); //     / |
				vertices.insert(vertices.end(), { v3.x, v3.y, v3.z, n3.x, n3.y, n3.z }); //    /__|

				// Triangle 2: v2-v4-v3
				vertices.insert(vertices.end(), { v2.x, v2.y, v2.z, n2.x, n2.y, n2.z });
				vertices.insert(vertices.end(), { v4.x, v4.y, v4.z, n4.x, n4.y, n4.z });
				vertices.insert(vertices.end(), { v3.x, v3.y, v3.z, n3.x, n3.y, n3.z });
			}
		}

		VAO vao(vertices);
		Vao = vao;
	}

	void draw() {
		Vao.bindVAO();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
	}

	void accelerate(float deltatime) {
		glm::vec3 accel = totalForce / mass;
		velocity += accel * deltatime;
		position += velocity * deltatime;

		totalForce = glm::vec3(0.0f);
	}
};