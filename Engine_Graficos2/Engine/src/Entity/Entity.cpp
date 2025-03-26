#include "Entity.h"
#include "RendererSingleton.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Entity::Entity()
{
	vBuffer = new unsigned int;
	iBuffer = new unsigned int;
	translation = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1 };
	UpdateModel(false);
}

Entity::~Entity()
{
	delete vBuffer;
	delete iBuffer;
}

void Entity::Translate(float x, float y, float z)
{
	translation.x += x;
	translation.y += y;
	translation.z += z;

	UpdateModel(true);
}

void Entity::RotateX(float angle)
{
	rotation.x += angle;

	UpdateModel(true);
}

void Entity::RotateY(float angle)
{
	rotation.y += angle;

	UpdateModel(true);
}

void Entity::RotateZ(float angle)
{
	rotation.z += angle;

	UpdateModel(true);
}

void Entity::Scale(float x, float y)
{
	scale.x += x;
	scale.y += y;

	if (scale.x < 0) scale.x = 0;
	if (scale.y < 0) scale.y = 0;

	UpdateModel(true);
}

Vector3 Entity::GetTranslation()
{
	return translation;
}

Vector3 Entity::GetRotation()
{
	return rotation;
}

Vector2 Entity::GetScale()
{
	return scale;
}

void Entity::UpdateModel(bool isModelCreated)
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));
	//glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (rotation * 3.14f) / 180, glm::vec3(0, 0, 1));
	//glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scal = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x / 2, scale.y / 2, 0.0f));

	glm::mat4 model = trans * rotX * rotY * rotZ * scal;

	if (isModelCreated)
	{
		RendererSingleton::GetRenderer()->SetModel(model, modelId);		
	}
	else
	{
		modelId = RendererSingleton::GetRenderer()->GetNewModelId(model);
	}
}
