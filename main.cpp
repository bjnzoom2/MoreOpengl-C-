#include <iostream>
#include <vector>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"

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

	int res = 50;

	std::vector<float> vertices = {};

	float posX = 0.0f;
	float posY = 0.0f;
	float rad = 0.25f;
	vertices.push_back(posX);
	vertices.push_back(posY);

	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	for (int i = 0; i <= res; i++) {
		float angle = 2.0f * 3.14159265359 * (static_cast<float>(i) / res);
		float x = posX + cos(angle) * rad;
		float y = posY + sin(angle) * rad;
		vertices.push_back(x);
		vertices.push_back(y);

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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		//shaderProgram.setVec4("color", { 1.0f, 0.0f, 0.0f, 1.0f });
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram.ID);
	glfwTerminate();

	return 0;
}