#pragma once
#include <string>
#include <vector>

#include "Window/Window.h"
#include "Program/Program.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "VertexArray.h"
#include "Material/Material.h"
#include "Light/Light.h"
#include <unordered_map>

class Renderer
{
private:
	Window* window;
	Program* lightingProgram;
	Program* spriteProgram;
	std::vector<glm::mat4> models;
	//glm::mat4 view;
	//glm::mat4 proj;

	unsigned int lightingShaderId;
	unsigned int spriteShaderId;

	std::vector<VertexBuffer*> vertexBuffers;
	std::vector<IndexBuffer*> indexBuffers;
	std::vector<VertexArray*> vertexArrays;

	//unsigned int vao;

	const int MAX_POINT_LIGHTS = 4;
	const int MAX_SPOT_LIGHTS = 2;

	std::unordered_map<std::string, unsigned int> textureCache;


public:
	Renderer(Window* window);
	~Renderer();

	void ClearScreen();
	void SwapWindowBuffers();
	void Draw(unsigned int vertexBuffer, unsigned int indexBuffer, unsigned int modelId, const glm::mat4& view, const glm::mat4& proj);
	void DrawRange(unsigned int vertexBuffer, unsigned int indexBuffer, unsigned int modelId, const glm::mat4& view, const glm::mat4& proj, unsigned int indexOffset, unsigned int indexCount);

	void DrawWithLighting(
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
		unsigned int diffuseTextureID = 0);

	void SetSpriteShaderActive();
	void SetLightingShaderActive();

	glm::mat4 GetModel(unsigned int modelId) const;
	void DeleteModel(unsigned int modelId);

	unsigned int GetNewVertexBuffer(const void* data, unsigned int size, bool is3D = false);
	void GetNewVertexBuffer(unsigned int bufferID, const void* data, unsigned int dataSize);
	unsigned int GetNewIndexBuffer(unsigned int* indices, unsigned int count);
	Window* GetWindow();

	Program* GetLightingProgram();
	Program* GetSpriteProgram();

	unsigned int GetNewModelId(glm::mat4 model);
	void SetModel(glm::mat4 model, unsigned int modelId);
	void GetNewSprite(std::string imgPath, int* width, int* height, int* bpp, unsigned int* imageID);
	void SetSprite(unsigned int value);
	void DeleteSprite(unsigned int* spriteID);
	void BindSprite(unsigned int slot, unsigned int spriteID);
	void UnbindSprite();
	void SetUniversalSpriteSettings();

	unsigned int LoadTexture(const std::string& path);

	void AddDirectionalLight(const DirectionalLight& light);
	void AddPointLight(const PointLight& light);
	void AddSpotLight(const SpotLight& light);
};
