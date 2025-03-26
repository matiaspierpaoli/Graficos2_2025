#include <iostream>

#include "Renderer.h"
#include "Buffers/IndexBuffer.h"
#include "glew/include/GL/glew.h"
#include <glfw/include/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <stb_image.h>

Renderer::Renderer(Window* window)
{
	this->window = window;

	program = new Program();

	glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);

	proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / window->GetHeight(), 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//unsigned int shader = program->CreateShader(program->ReadFile("shaders/vertexShader.shader"), program->ReadFile("shaders/fragmentShader.shader"));
	unsigned int shader = program->CreateShader(program->ReadFile("shaders/vertexShaderSprite.shader"), program->ReadFile("shaders/fragmentShaderSprite.shader"));
	glUseProgram(shader);

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);

	SetUniversalSpriteSettings();
}

Renderer::~Renderer()
{
	delete window;
	delete program;

	for (int i = 0; i < vertexArrays.size(); i++)
		delete vertexArrays[i];
	for (int i = 0; i < indexBuffers.size(); i++)
		delete indexBuffers[i];
}

void Renderer::ClearScreen()
{
	glViewport(0, 0, window->GetWidth(), window->GetHeight());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapWindowBuffers()
{
	if (!window || !window->WindowExists())
		std::cout << "RENDERER ERROR: Window does not exist" << std::endl;

	glfwSwapBuffers((GLFWwindow*)window->GetGLFWPointer());
}

void Renderer::Draw(unsigned int vertexBuffer, unsigned int indexBuffer, unsigned int modelId)
{
	VertexBuffer* vb = vertexBuffers[vertexBuffer];
	IndexBuffer* ib = indexBuffers[indexBuffer];
	VertexArray* va = vertexArrays[vertexBuffer];

	vb->Bind();
	va->Bind();
	ib->Bind();

	program->SetUniformMat4F("mvp", proj * view * models[modelId]);
	//program->SetUniform4f("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//texture->Bind();
	//program->SetUniform1i("uTexture", 0);

	glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}

unsigned int Renderer::GetNewVertexBuffer(const void* data, unsigned int size)
{
	unsigned int bufferID = vertexArrays.size();

	VertexBuffer* vb = new VertexBuffer(data, size, true);
	vertexBuffers.push_back(vb);

	VertexBufferLayout layout;
	layout.Push<float>(2); // Position
	//layout.Push<float>(4); // Color
	layout.Push<float>(2); // UV 
	//layout.Push<float>(4); //Color

	VertexArray* va = new VertexArray();
	va->AddBuffer(vb, layout);

	vertexArrays.push_back(va);

	return bufferID;
}

void Renderer::GetNewVertexBuffer(unsigned int bufferID, const void* data, unsigned int dataSize)
{
	VertexBuffer* vb = vertexBuffers[bufferID];

	vb->Bind();
	vb->SetBuffer(data, dataSize);

	vertexArrays[bufferID]->SetBuffer(bufferID);
}

unsigned int Renderer::GetNewIndexBuffer(unsigned int* indices, unsigned int indexAmmount)
{
	unsigned int bufferID = indexBuffers.size();

	IndexBuffer* ib = new IndexBuffer(indices, indexAmmount);
	indexBuffers.push_back(ib);

	return bufferID;
}

unsigned int Renderer::GetNewModelId(glm::mat4 model)
{
	unsigned int newModelId = models.size();
	models.push_back(model);

	return newModelId;
}

void Renderer::SetModel(glm::mat4 model, unsigned int modelId)
{
	models[modelId] = model;
}

void Renderer::GetNewSprite(std::string imgPath, int* imgWidth, int* imgHeight, int* bpp, unsigned int* imageID)
{
	unsigned char* localBuffer = nullptr;

	stbi_set_flip_vertically_on_load(1);

	//bpp = Bits per Pixel
	localBuffer = stbi_load(imgPath.c_str(), imgWidth, imgHeight, bpp, 4);

	glGenTextures(1, imageID);
	glBindTexture(GL_TEXTURE_2D, *imageID);

	//If image didn't load exit
	if (!localBuffer) return;

	//https://docs.gl/gl4/glTexImage2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, *imgWidth, *imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Free buffer/image from stbi
	stbi_image_free(localBuffer);
}

void Renderer::SetSprite(unsigned int value)
{
	program->SetUniform1i("u_Sprite", value);
}

void Renderer::DeleteSprite(unsigned int* spriteID)
{
	glDeleteTextures(1, spriteID);
}

void Renderer::BindSprite(unsigned int slot, unsigned int spriteID)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, spriteID);
}

void Renderer::UnbindSprite()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::SetUniversalSpriteSettings()
{
	//https://docs.gl/gl4/glTexParameteri
	///SET MIPMAPPING //vaRS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	///SET WRAPPING //vaRS
	//Repeat: repeats image in empty space
	//Mirror Repeat: repeats image, but mirroring it
	//Clamp Border: stretches image to edge of screen
	//Clamp Edge: fills empty space left by image with color
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Enable blending, so images with transparency can be draw
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}