#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Exports.h"
#include "Camera/Camera.h"
#include "Light/Light.h"
#include <../libs/glm/glm.hpp>
#include <vector>
#include <string>

struct TransformMatrix;

class GraficosEngine_API Entity
{
protected:
	Vector3 translation;
	Vector3 rotation;
	Vector3 scale;
	unsigned int modelId;

	std::string name;
	Entity* parent;
	std::vector<Entity*> children;

public:
	Entity();
	virtual ~Entity();

	unsigned int* vBuffer;
	unsigned int* iBuffer;

	void Translate(float x, float y, float z);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(float x, float y, float z);

	Vector3 GetTranslation() const;
	Vector3 GetRotation() const;
	Vector3 GetScale()const ;
	unsigned int GetModelId() const;

	virtual void UpdateModel(bool isModelCreated);
	virtual glm::mat4 GetTransformMatrix() const;

	// Hierarchy
	void SetName(const std::string& name);
	const std::string& GetName() const;
	void AddChild(Entity* child);
	Entity* GetParent() const;
	const std::vector<Entity*>& GetChildren() const;
	Entity* FindChildByName(const std::string& name, bool recursive = true);

	glm::mat4 CalculateLocalTransform() const;

	virtual void Render(Camera* camera,
		const std::vector<DirectionalLight>& dirLights,
		const std::vector<PointLight>& pointLights,
		const std::vector<SpotLight>& spotLights);
};

