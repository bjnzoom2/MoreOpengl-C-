#include <iostream>
#include <vector>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "camera.h"
#include "obj.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;
const float G_CONST = 6.67430e-11;

std::vector<Object> objs = {};
Camera camera({ 0.0f, 0.0f, 3.0f });
bool firstMouse = true;
float lastX = 400, lastY = 300;

void processInput(GLFWwindow* window, float deltatime)
{
	const float cameraSpeed = 2.0f * deltatime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, (int)true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.cameraPos += cameraSpeed * camera.cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.cameraPos -= cameraSpeed * camera.cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.yaw += xoffset;
	camera.pitch += yoffset;

	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	direction.y = sin(glm::radians(camera.pitch));
	direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.cameraDir = direction;
	camera.cameraFront = glm::normalize(camera.cameraDir);
	camera.cameraRight = glm::normalize(glm::cross(camera.up, camera.cameraDir));
	camera.cameraUp = glm::cross(camera.cameraDir, camera.cameraRight);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "C++ Opengl", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to load window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);

	/*unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	unsigned int color = glGetUniformLocation(shaderProgram, "color");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/;
	std::filesystem::path vertexPath{ R"(C:\Users\luken\source\repos\MoreOpengl C++\vertex.glsl)" };
	std::filesystem::path fragmentPath{ R"(C:\Users\luken\source\repos\MoreOpengl C++\fragment.glsl)" };

	Shader shaderProgram(vertexPath.string().c_str(), fragmentPath.string().c_str());

	/*int res = 50;

	std::vector<float> vertices = {};
	glm::vec2 position(0.0f, 0.875f);
	glm::vec2 velocity(0.0f, 0.0f);
	glm::vec2 acceleration(0.0f, 0.0f);

	float rad = 0.125f;
	vertices.push_back(position.x);
	vertices.push_back(position.y);
	vertices.push_back(0.0f);

	vertices.push_back(1.0f);	
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	for (int i = 0; i <= res; i++) {
		float angle = 2.0f * 3.14159265359 * (static_cast<float>(i) / res);
		float x = position.x + cos(angle) * rad;
		float y = position.y + sin(angle) * rad;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);

		vertices.push_back(1.0f);
		vertices.push_back(1.0f);	
		vertices.push_back(1.0f);
	}

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); */

	Object obj1({ -0.5f, 0.0f, -0.5f }, { 0.1086f, 0.0f, -0.1086f }, 1e9, 0.125f);
	Object obj2({ 0.5f, 0.0f, 0.5f }, { -0.1086f, 0.0f, 0.1086f }, 1e9, 0.125f);
	objs.push_back(obj1);
	objs.push_back(obj2);

	float deltatime;
	float currentTime = glfwGetTime();
	float previousTime = 0.0f;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	while (!glfwWindowShouldClose(window)) {
		deltatime = currentTime - previousTime;
		previousTime = currentTime;
		currentTime = glfwGetTime();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.use();
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.up);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setMat4("view", view);

		for (int i = 0; i < objs.size(); i++) {
			Object& obj = objs[i];
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, obj.position);
			shaderProgram.setMat4("model", model);
			obj.draw();

			for (int j = i + 1; j < objs.size(); j++) {
				Object& obj_2 = objs[j];
				float dist2 = glm::distance(obj.position, obj_2.position) * glm::distance(obj.position, obj_2.position);
				float gForce = G_CONST * (obj.mass * obj_2.mass / dist2);
				glm::vec3 dir = glm::normalize(obj_2.position - obj.position);

				obj.totalForce += gForce * dir;
				obj_2.totalForce -= gForce * dir;
			}
			obj.accelerate(deltatime);
			obj.totalForce = glm::vec3(0.0f);
		}

		processInput(window, deltatime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	for (auto& obj : objs) {
		glDeleteVertexArrays(1, &obj.Vao.vao);
		glDeleteBuffers(1, &obj.Vao.vbo);
	}
	glDeleteProgram(shaderProgram.ID);
	glfwTerminate();

	return 0;
}