#include "VertexBufferLayout.h"
#include <glew/include/GL/glew.h>

template<typename T>
void VertexBufferLayout::Push(unsigned int count)
{
	//static_assert(false);
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
	elements.push_back({ GL_FLOAT, count, GL_FALSE });
	stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}