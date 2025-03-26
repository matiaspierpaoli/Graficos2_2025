#include "Buffers/VertexBuffer.h"

#include "glew/include/GL/glew.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool dataIsStatic)
{
	//Ask openGL for X buffers (1 in this case) and links them to a uint pointer
	//https://docs.gl/gl4/glGenBuffers
	glGenBuffers(1, &rendererID);

	//Select buffer and set it as array buffer (ideal to work with vertexes)
	//https://docs.gl/gl4/glBindBuffer
	//unsigned int vBufferData = *id;
	glBindBuffer(GL_ARRAY_BUFFER, rendererID); //maybe problems?

	//Send data to buffer
	//https://docs.gl/gl4/glBufferData
	GLenum dataUsage = dataIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	//unsigned int vDataSize = vAmount * stride;
	glBufferData(GL_ARRAY_BUFFER, size, data, dataUsage); //may be a problem
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::SetBuffer(const void* data, unsigned int size)
{
	//Send data to buffer
	//https://docs.gl/gl4/glBufferData
	bool dataIsStatic = false;
	GLenum dataUsage = dataIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	//unsigned int vDataSize = vAmount * stride;
	glBufferData(GL_ARRAY_BUFFER, size, data, dataUsage); //may be a problem
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}