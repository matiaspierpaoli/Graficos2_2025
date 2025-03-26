#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Exports.h"

class GraficosEngine_API Entity
{
private:
	Vector3 translation;
	Vector3 rotation;
	Vector2 scale;

public:
	Entity();
	~Entity();

	unsigned int* vBuffer;
	unsigned int* iBuffer;
	unsigned int modelId;

	void Translate(float x, float y, float z);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(float x, float y);

	Vector3 GetTranslation();
	Vector3 GetRotation();
	Vector2 GetScale();

	void UpdateModel(bool isModelCreated);
};

