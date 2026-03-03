#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

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

	std::vector<float> vertices = {};

	float posX = 0.0f;
	float posY = 0.0f;
	float rad = 0.5f;
	vertices.push_back(posX);
	vertices.push_back(posY);
	for (int i = 0; i <= 50; i++) {
		float angle = 2.0f * 3.14159265359 * (static_cast<float>(i) / 50);
		float x = posX + cos(angle) * rad;
		float y = posY + sin(angle) * rad;
		vertices.push_back(x);
		vertices.push_back(y);
	}

	for (int i = 0; i < vertices.size(); i++) {
		std::cout << vertices[i] << '\n';
	}

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}