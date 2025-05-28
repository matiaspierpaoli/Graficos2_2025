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
	std::vector<SpotLight> activeSpotLights,
	unsigned int diffuseTextureID
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

	// Material
	lightingProgram->SetUniform3f("materialAmbient", material.ambient.r, material.ambient.g, material.ambient.b);
	lightingProgram->SetUniform3f("materialDiffuse", material.diffuse.r, material.diffuse.g, material.diffuse.b);
	lightingProgram->SetUniform3f("materialSpecular", material.specular.r, material.specular.g, material.specular.b);
	lightingProgram->SetUniform1f("materialShininess", material.shininess);

	// Directional lights
	if (!activeDirLights.empty()) {
		const DirectionalLight& light = activeDirLights[0];

		std::string prefix = "dirLight.";
		lightingProgram->SetUniform3f(prefix + "direction", light.direction.x, light.direction.y, light.direction.z);
		lightingProgram->SetUniform3f(prefix + "color", light.color.r, light.color.g, light.color.b);
	}
	
	// Point lights
	lightingProgram->SetUniform1i("numPointLights", std::min(static_cast<int>(activePointLights.size()), MAX_POINT_LIGHTS));

	if (!activePointLights.empty()) {
		
		for (int i = 0; i < activePointLights.size(); i++)
		{
			const PointLight& light = activePointLights[i];

			std::string prefix = "pointLights[" + std::to_string(i) + "].";
			lightingProgram->SetUniform3f(prefix + "position", light.position.x, light.position.y, light.position.z);
			lightingProgram->SetUniform3f(prefix + "color", light.color.r, light.color.g, light.color.b);
			lightingProgram->SetUniform1f(prefix + "constant", light.constant);
			lightingProgram->SetUniform1f(prefix + "linear", light.linear);
			lightingProgram->SetUniform1f(prefix + "quadratic", light.quadratic);
		}
		
	}

	// Spot lights
	lightingProgram->SetUniform1i("numSpotLights", std::min(static_cast<int>(activeSpotLights.size()), MAX_SPOT_LIGHTS));

	if (!activeSpotLights.empty()) {

		for (int i = 0; i < activeSpotLights.size(); i++)
		{
			const SpotLight& spot = activeSpotLights[i];

			std::string prefix = "spotLights[" + std::to_string(i) + "].";
			lightingProgram->SetUniform3f(prefix + "position", spot.position.x, spot.position.y, spot.position.z);
			lightingProgram->SetUniform3f(prefix + "direction", spot.direction.x, spot.direction.y, spot.direction.z);
			lightingProgram->SetUniform3f(prefix + "color", spot.color.r, spot.color.g, spot.color.b);
			lightingProgram->SetUniform1f(prefix + "cutOff", glm::cos(glm::radians(spot.cutOff)));
			lightingProgram->SetUniform1f(prefix + "outerCutOff", glm::cos(glm::radians(spot.outerCutOff)));
			lightingProgram->SetUniform1f(prefix + "constant", spot.constant);
			lightingProgram->SetUniform1f(prefix + "linear", spot.linear);
			lightingProgram->SetUniform1f(prefix + "quadratic", spot.quadratic);
		}
	}

	if (material.useTexture) {
		BindSprite(0, material.diffuseTexture);
		lightingProgram->SetUniform1i("diffuseTexture", 0);
		lightingProgram->SetUniform1i("useTexture", 1); // Textura activada
	}
	else {
		lightingProgram->SetUniform1i("useTexture", 0); // Textura desactivada
	}


	// Camera
	lightingProgram->SetUniform3f("viewPos", camPos.x, camPos.y, camPos.z);

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
		layout.Push<float>(2); // UV
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	///SET WRAPPING //vaRS
	//Repeat: repeats image in empty space
	//Mirror Repeat: repeats image, but mirroring it
	//Clamp Border: stretches image to edge of screen
	//Clamp Edge: fills empty space left by image with color
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Enable blending, so images with transparency can be draw
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int Renderer::LoadTexture(const std::string& path)
{
	if (textureCache.find(path) != textureCache.end()) {
		return textureCache[path];
	}

	int width, height, bpp;
	unsigned int textureID;
	GetNewSprite(path, &width, &height, &bpp, &textureID);

	if (textureID == 0) {
		std::cerr << "ERROR: No se pudo cargar la textura en: " << path << std::endl;
	}
	else {
		std::cout << "Textura cargada correctamente: " << path
			<< " (ID: " << textureID << ")" << std::endl;
	}


	textureCache[path] = textureID;
	return textureID;
}
