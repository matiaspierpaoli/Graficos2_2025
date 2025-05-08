#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Exports.h"
#include <../libs/glm/glm.hpp>

struct TransformMatrix;

class GraficosEngine_API Entity
{
protected:
	Vector3 translation;
	Vector3 rotation;
	Vector3 scale;
	unsigned int modelId;

public:
	Entity();
	~Entity();

	unsigned int* vBuffer;
	unsigned int* iBuffer;

	void Translate(float x, float y, float z);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(float x, float y, float z);

	Vector3 GetTranslation();
	Vector3 GetRotation();
	Vector3 GetScale();
	unsigned int GetModelId();

	virtual void UpdateModel(bool isModelCreated);
	glm::mat4 GetTransformMatrix() const;
};

