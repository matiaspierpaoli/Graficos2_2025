#include "Buffers/IndexBuffer.h"

#include "glew/include/GL/glew.h"
#include "Renderer/Renderer.h"

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
	: count(count)
{
	assert(sizeof(unsigned int) == sizeof(GLuint));

	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW); //may be a problem
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}