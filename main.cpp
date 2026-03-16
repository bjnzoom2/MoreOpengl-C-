#include <iostream>
#include <vector>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

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

	std::filesystem::path vertexPath{ R"(C:\Users\luken\source\repos\MoreOpengl C++\vertex.glsl)" };
	std::filesystem::path fragmentPath{ R"(C:\Users\luken\source\repos\MoreOpengl C++\fragment.glsl)" };

	Shader shaderProgram(vertexPath.string().c_str(), fragmentPath.string().c_str());

	std::filesystem::path texturePath{ R"(C:\Users\luken\source\repos\MoreOpengl C++\Textures\Uranus.png)" };
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.string().c_str(), &width, &height, &nrChannels, 0);
	
	Object obj1({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1e10, 0.25f, Material(), true, data, width, height, nrChannels);
	Object obj2({ 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.817f }, 1e6, 0.125f, Material(glm::vec3(0.0f, 1.0f, 1.0f) * 0.25f, glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.5f), 4.0f), false, data, width, height, nrChannels);
	Object obj3({ 2.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.57768f }, 1e6, 0.125f, Material(glm::vec3(1.0f, 1.0f, 0.0f) * 0.25f, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5f), 4.0f), false, data, width, height, nrChannels);
	Object obj4({ 3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.4717f }, 1e6, 0.125f, Material(glm::vec3(1.0f, 0.0f, 0.0f) * 0.25f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f), 4.0f), false, data, width, height, nrChannels);
	objs.push_back(obj1);
	objs.push_back(obj2);
	objs.push_back(obj3);
	objs.push_back(obj4);

	if (data) stbi_image_free(data);

	//Light light({0.0f, 0.0f, 0.0f}, 0.075f);

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
			shaderProgram.setMaterial("material", obj.material);
			if (obj.isLight) {
				shaderProgram.setVec3("lightColor", obj.material.diffuse);
				shaderProgram.setVec3("lightPos", obj.position);
			}

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
		}

		/*lightShaderProgram.use();
		lightShaderProgram.setMat4("projection", projection);
		lightShaderProgram.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, light.position);
		lightShaderProgram.setMat4("model", model);

		light.draw();*/

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