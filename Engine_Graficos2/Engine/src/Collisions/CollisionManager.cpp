#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::checkEntityToEntityCollision(Entity2D* entity1, Entity2D* entity2)
{
	if (entity1->GetTranslation().x + entity1->GetScale().x / 2 >=  //entity1 right edge past entity2 left
		entity2->GetTranslation().x - entity2->GetScale().x / 2 &&
		entity1->GetTranslation().x - entity1->GetScale().x / 2 <=  //e1 left edge past e2 right
		entity2->GetTranslation().x + entity2->GetScale().x / 2 &&
		entity1->GetTranslation().y + entity1->GetScale().y / 2 >=  //e1 top edge past e2 bottom
		entity2->GetTranslation().y - entity2->GetScale().y / 2 &&
		entity1->GetTranslation().y - entity1->GetScale().y / 2 <=  //e1 bottom edge past e2 top
		entity2->GetTranslation().y + entity2->GetScale().y / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//bool CollisionManager::checkEntityToWindowCollision(Entity2D* entity, Window* window)
//{
//	if (entity->GetTranslation().x - entity->GetScale().x / 2 < 0 ||
//		entity->GetTranslation().x + entity->GetScale().x / 2 > window->GetWidth() ||
//		entity->GetTranslation().y - entity->GetScale().y / 2 < 0 ||
//		entity->GetTranslation().y + entity->GetScale().y / 2 > window->GetHeight())
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

bool CollisionManager::checkEntityToWindowCollision(Entity2D* entity, Window* window)
{
	float leftEdge = entity->GetTranslation().x - (entity->GetScale().x / 2);
	float rightEdge = entity->GetTranslation().x + (entity->GetScale().x / 2);
	float topEdge = entity->GetTranslation().y - (entity->GetScale().y / 2);
	float bottomEdge = entity->GetTranslation().y + (entity->GetScale().y / 2);

	if (leftEdge >= 0 && rightEdge <= window->GetWidth() &&
		topEdge >= 0 && bottomEdge <= window->GetHeight())
	{
		return false; // No collision with window
	}
	else
	{
		return true; // Collision with window
	}
}