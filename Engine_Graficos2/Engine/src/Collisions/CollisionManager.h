#pragma once
#include "Exports.h"

#include "Entity/Entity2D/Entity2D.h"
#include "Window/Window.h"

class GraficosEngine_API CollisionManager
{
private:

public:
	CollisionManager();
	~CollisionManager();

	bool checkEntityToEntityCollision(Entity2D* entity1, Entity2D* entity2);
	bool checkEntityToWindowCollision(Entity2D* entity, Window* window);
};