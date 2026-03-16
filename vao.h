#pragma once
#include <vector>
#include <glad/glad.h>
#include "texture.h"

class VAO {
private:
	std::vector<float> vertices;
public:
	unsigned int vao, vbo;
	unsigned char* data;
	Texture texture;
	VAO() : vertices(), vao(0), vbo(0), data(nullptr) {}
	VAO(std::vector<float> vert, unsigned char* _data, int width, int height, int nrChannels) : vertices(vert), data(_data) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		Texture Texture(data, width, height, nrChannels);
		texture = Texture;
	}

	void bindVAO() {
		glBindTexture(GL_TEXTURE_2D, texture.texture);
		glBindVertexArray(vao);
	}
};