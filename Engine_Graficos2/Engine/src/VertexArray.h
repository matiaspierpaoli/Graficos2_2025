#pragma once
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int rendererID;
	std::vector<VertexBuffer*> vertexBuffers;
	std::vector<VertexBufferLayout> layouts;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer* vb, const VertexBufferLayout& layout);
	void SetBuffer(unsigned int bufferID);

	void Bind() const;
	void UnBind() const;
};
