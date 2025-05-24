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

	lightingProgram = new Program();
	spriteProgram = new Program();	

	glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);

	/*proj = glm::perspective(glm::radians(45.0f), window->GetWidth() / window->GetHeight(), 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

	//unsigned int shader = program->CreateShader(program->ReadFile("shaders/vertexShader.shader"), program->ReadFile("shaders/fragmentShader.shader"));
	lightingShaderId = lightingProgram->CreateShader(
		lightingProgram->ReadFile("shaders/vertexLighting.shader"),
		lightingProgram->ReadFile("shaders/fragmentLighting.shader")
	);

	spriteShaderId = spriteProgram->CreateShader(
		spriteProgram->ReadFile("shaders/vertexShaderSprite.shader"),
		spriteProgram->ReadFile("shaders/fragmentShaderSprite.shader")
	);

	// Usar por default el de sprite para UI
	glUseProgram(spriteShaderId);

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glDepthFunc(GL_LESS);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SetUniversalSpriteSettings();
}

Renderer::~Renderer()
{
	delete window;
	delete lightingProgram;
	delete spriteProgram;

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

void Renderer::Draw(unsigned int vertexBuffer, unsigned int indexBuffer, unsigned int modelId, const glm::mat4& view, const glm::mat4& proj)
{
	VertexBuffer* vb = vertexBuffers[vertexBuffer];
	IndexBuffer* ib = indexBuffers[indexBuffer];
	VertexArray* va = vertexArrays[vertexBuffer];

	vb->Bind();
	va->Bind();
	ib->Bind();

	spriteProgram->SetUniformMat4F("mvp", proj * view * models[modelId]);
	//program->SetUniform4f("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//texture->Bind();
	//program->SetUniform1i("uTexture", 0);

	glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawRange(
	unsigned int vertexBuffer,
	unsigned int indexBuffer,
	unsigned int modelId,
	const glm::mat4& view,
	const glm::mat4& proj,
	unsigned int indexOffset,
	unsigned int indexCount)
{
	VertexBuffer* vb = vertexBuffers[vertexBuffer];
	IndexBuffer* ib = indexBuffers[indexBuffer];
	VertexArray* va = vertexArrays[vertexBuffer];

	vb->Bind();
	va->Bind();
	ib->Bind();

	spriteProgram->SetUniformMat4F("mvp", proj * view * models[modelId]);

	// Dibujar solo un rango de índices
	glDrawElements(
		GL_TRIANGLES,
		indexCount,
		GL_UNSIGNED_INT,
		(void*)(indexOffset * sizeof(unsigned int))
	);
}

void Renderer::DrawWithLighting(
	unsigned int vertexBuffer,
	unsigned int indexBuffer,
	unsigned int modelId,
	const glm::mat4& view,
	const glm::mat4& proj,
	const Material& material,
	glm::vec3 camPos,
	std::vector<DirectionalLight> activeDirLights,
	std::vector<PointLight> activePointLights,
	std::vector<SpotLight> activeSpotLights
)
{
	VertexBuffer* vb = vertexBuffers[vertexBuffer];
	IndexBuffer* ib = indexBuffers[indexBuffer];
	VertexArray* va = vertexArrays[vertexBuffer];

	vb->Bind();
	va->Bind();
	ib->Bind();

	// Matrices
	glm::mat4 model = models[modelId];
	lightingProgram->SetUniformMat4F("model", model);
	lightingProgram->SetUniformMat4F("view", view);
	lightingProgram->SetUniformMat4F("projection", proj);

	// Cámara
	lightingProgram->SetUniform3f("viewPos", camPos.x, camPos.y, camPos.z);

	// Material
	lightingProgram->SetUniform3f("material.ambient", material.ambient.r, material.ambient.g, material.ambient.b);
	lightingProgram->SetUniform3f("material.diffuse", material.diffuse.r, material.diffuse.g, material.diffuse.b);
	lightingProgram->SetUniform3f("material.specular", material.specular.r, material.specular.g, material.specular.b);
	lightingProgram->SetUniform1f("material.shininess", material.shininess);

	// Direccional: usar solo la primera
	if (!activeDirLights.empty()) {
		const DirectionalLight& dl = activeDirLights[0];
		lightingProgram->SetUniform3f("dirLight.direction", dl.direction.x, dl.direction.y, dl.direction.z);
		lightingProgram->SetUniform3f("dirLight.color", dl.color.r, dl.color.g, dl.color.b);
		lightingProgram->SetUniform1f("dirLight.intensity", dl.intensity);
	}

	// Point Lights
	for (size_t i = 0; i < std::min(activePointLights.size(), size_t(4)); i++) {
		const PointLight& pl = activePointLights[i];
		std::string prefix = "pointLights[" + std::to_string(i) + "].";
		lightingProgram->SetUniform3f(prefix + "position", pl.position.x, pl.position.y, pl.position.z);
		lightingProgram->SetUniform3f(prefix + "color", pl.color.r, pl.color.g, pl.color.b);
		lightingProgram->SetUniform1f(prefix + "intensity", pl.intensity);
		lightingProgram->SetUniform1f(prefix + "constant", pl.constant);
		lightingProgram->SetUniform1f(prefix + "linear", pl.linear);
		lightingProgram->SetUniform1f(prefix + "quadratic", pl.quadratic);
	}

	// Spot Lights
	for (size_t i = 0; i < std::min(activeSpotLights.size(), size_t(2)); i++) {
		const SpotLight& sl = activeSpotLights[i];
		std::string prefix = "spotLights[" + std::to_string(i) + "].";
		lightingProgram->SetUniform3f(prefix + "position", sl.position.x, sl.position.y, sl.position.z);
		lightingProgram->SetUniform3f(prefix + "direction", sl.direction.x, sl.direction.y, sl.direction.z);
		lightingProgram->SetUniform3f(prefix + "color", sl.color.r, sl.color.g, sl.color.b);
		lightingProgram->SetUniform1f(prefix + "intensity", sl.intensity);
		lightingProgram->SetUniform1f(prefix + "cutOff", sl.cutOff);
		lightingProgram->SetUniform1f(prefix + "outerCutOff", sl.outerCutOff);
	}

	glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer::SetSpriteShaderActive()
{
	glUseProgram(spriteShaderId);
}

void Renderer::SetLightingShaderActive()
{
	glUseProgram(lightingShaderId);
}


glm::mat4 Renderer::GetModel(unsigned int modelId) const
{
	if (modelId < models.size()) {
		return models[modelId];
	}
	return glm::mat4(1.0f); // Retorna matriz identidad si el ID no es válido
}

void Renderer::DeleteModel(unsigned int modelId)
{
	if (modelId < models.size()) {
		models[modelId] = glm::mat4(1.0f); // Reset a matriz identidad
	}
}

unsigned int Renderer::GetNewVertexBuffer(const void* data, unsigned int size, bool is3D)
{
	unsigned int bufferID = vertexArrays.size();

	VertexBuffer* vb = new VertexBuffer(data, size, true);
	vertexBuffers.push_back(vb);

	VertexBufferLayout layout;
	if (is3D)
	{
		layout.Push<float>(3); // Position 3D
		layout.Push<float>(3); // normal
	}
	else
	{
		layout.Push<float>(2); // Position 2D
		layout.Push<float>(2); // UV 
	}

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

Window* Renderer::GetWindow()
{
	return window;
}

Program* Renderer::GetLightingProgram()
{
	return lightingProgram;
}

Program* Renderer::GetSpriteProgram()
{
	return spriteProgram;
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
	spriteProgram->SetUniform1i("u_Sprite", value);
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
