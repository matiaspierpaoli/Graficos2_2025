#pragma once
#include "Entity/Entity2D/Shape/Shape.h"
#include "Exports.h"

class GraficosEngine_API Triangle : public Shape
{
public:
	Triangle();
	~Triangle();

	void Draw();
};

