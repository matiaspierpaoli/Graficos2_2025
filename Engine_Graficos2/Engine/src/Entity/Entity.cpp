#include "Entity.h"
#include "RendererSingleton.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TransformMatrix {
	glm::mat4 matrix;

	// Operador de conversión explícito a glm::mat4
	explicit operator glm::mat4() const { return matrix; }

	// Alternativamente, puedes usar un método explícito:
	glm::mat4 asGlmMatrix() const { return matrix; }
};

Entity::Entity()
{
	vBuffer = new unsigned int;
	iBuffer = new unsigned int;
	translation = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };
	UpdateModel(false);
}

Entity::~Entity()
{
	RendererSingleton::GetRenderer()->DeleteModel(modelId);
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

void Entity::Scale(float x, float y, float z)
{
	scale.x = glm::max(0.0f, scale.x + x);
	scale.y = glm::max(0.0f, scale.y + y);
	scale.z = glm::max(0.0f, scale.z + z);
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

Vector3 Entity::GetScale()
{
	return scale;
}

unsigned int Entity::GetModelId()
{
	return modelId;
}

void Entity::UpdateModel(bool isModelCreated)
{
	glm::mat4 model = GetTransformMatrix();

	if (isModelCreated)
	{
		RendererSingleton::GetRenderer()->SetModel(model, modelId);
	}
	else
	{
		modelId = RendererSingleton::GetRenderer()->GetNewModelId(model);
	}
}

glm::mat4 Entity::GetTransformMatrix() const
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f),
		glm::vec3(translation.x, translation.y, translation.z));

	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f),
		glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f),
		glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f),
		glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 scal = glm::scale(glm::mat4(1.0f),
		glm::vec3(scale.x, scale.y, scale.z));

	return trans * rotY * rotX * rotZ * scal;
}
