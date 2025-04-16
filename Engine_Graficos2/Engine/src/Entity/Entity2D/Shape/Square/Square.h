#pragma once
#include "Entity/Entity2D/Shape/Shape.h"
#include "Exports.h"
#include "../libs/glm/glm.hpp"

class  GraficosEngine_API Square : public Shape
{
public:
	Square(float vertexCol[4][4]);
	~Square();

	void Draw(const glm::mat4& view, const glm::mat4& proj);
};
