#pragma once
#include <vector>
#include "VertexBufferElement.h"

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout() : stride(0) {}

	template<typename T>
	void Push(unsigned int count);

	template<>
	void Push<float>(unsigned int count);

	template<>
	void Push<unsigned int>(unsigned int count);

	template<>
	void Push<unsigned char>(unsigned int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return elements; };
	inline unsigned int GetStride() const { return stride; };
};