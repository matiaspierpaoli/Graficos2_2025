#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:
	IndexBuffer(unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const { return count; };
};
