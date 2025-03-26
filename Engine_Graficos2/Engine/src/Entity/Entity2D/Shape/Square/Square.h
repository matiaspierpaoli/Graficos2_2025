#pragma once
#include "Entity/Entity2D/Shape/Shape.h"
#include "Exports.h"

class  GraficosEngine_API Square : public Shape
{
public:
	Square(float vertexCol[4][4]);
	~Square();

	void Draw();
};
