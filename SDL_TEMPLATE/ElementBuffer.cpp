#include "ElementBuffer.h"
#include <glad/glad.h>

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int pCount) : count(pCount) {
	// ASSERT(sizeof(unsigned int) == sizeof(GLuint))

	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pCount * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &rendererID);
}

void ElementBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void ElementBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}