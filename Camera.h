#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 cameraPos = {};
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraDir = {};
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = {};
	glm::vec3 cameraUp = {};

	float pitch = 0.0f;
	float yaw = -90.0f;

	Camera(glm::vec3 camPos) : cameraPos(camPos) {
		cameraFront = glm::normalize(cameraDir);
		cameraRight = glm::normalize(glm::cross(up, cameraDir));
		cameraUp = glm::cross(cameraDir, cameraRight);
	}
};