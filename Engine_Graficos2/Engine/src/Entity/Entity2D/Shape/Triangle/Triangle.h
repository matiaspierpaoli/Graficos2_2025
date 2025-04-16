#pragma once
#include "Entity/Entity2D/Shape/Shape.h"
#include "Exports.h"
#include "../libs/glm/glm.hpp"

class GraficosEngine_API Triangle : public Shape
{
public:
	Triangle();
	~Triangle();

	void Draw(const glm::mat4& view, const glm::mat4& proj);
};

