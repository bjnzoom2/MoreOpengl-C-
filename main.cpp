#include <iostream>
#include <vector>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "obj.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

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

	glm::vec2 position(0.0f, 0.875f);
	glm::vec2 velocity(0.0f, 0.0f);
	glm::vec2 acceleration(0.0f, 0.0f);
	Object obj(position, velocity, acceleration, 1, 0.125f);

	float deltatime;
	float currentTime = glfwGetTime();
	float previousTime = 0;

	while (!glfwWindowShouldClose(window)) {
		deltatime = currentTime - previousTime;
		previousTime = currentTime;
		currentTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.use();
		//shaderProgram.setVec4("color", { 1.0f, 0.0f, 0.0f, 1.0f });
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(obj.velocity, 0.0f));
		shaderProgram.setMat4("transform", trans);

		obj.velocity.x += obj.acceleration.x * deltatime;
		obj.velocity.y += obj.acceleration.y * deltatime;

		obj.acceleration.y -= 1 * deltatime;
		
		obj.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &obj.Vao.vao);
	glDeleteBuffers(1, &obj.Vao.vbo);
	glDeleteProgram(shaderProgram.ID);
	glfwTerminate();

	return 0;
}