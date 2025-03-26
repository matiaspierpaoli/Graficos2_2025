#include "VertexArray.h"
#include <glew/include/GL/glew.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray()
{
	for (unsigned short i = 0; i < vertexBuffers.size(); i++)
	{
		delete vertexBuffers[i];
	}

	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::AddBuffer(VertexBuffer* vb, const VertexBufferLayout& layout)
{
	vertexBuffers.push_back(vb);
	layouts.push_back(layout);

	Bind();

	vb->Bind();

	unsigned int offset = 0;
	const auto& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];


		//https://docs.gl/gl4/glEnableVertexAttribArray
		glEnableVertexAttribArray(i);

		//SET EVERY CHUNK OF THE VERTEX
		//https://docs.gl/gl4/glVertexAttribPointer
		unsigned int stride = layout.GetStride();
		glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::SetBuffer(unsigned int bufferID)
{
	Bind();

	vertexBuffers[0]->Bind(); //HERE BREAKS

	unsigned int offset = 0;
	VertexBufferLayout layout = layouts[0];
	const auto& elements = layout.GetElements();

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];


		//https://docs.gl/gl4/glEnableVertexAttribArray
		glEnableVertexAttribArray(i);

		//SET POS
		//https://docs.gl/gl4/glVertexAttribPointer
		unsigned int stride = layout.GetStride();
		glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}


void VertexArray::Bind() const
{
	glBindVertexArray(rendererID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
