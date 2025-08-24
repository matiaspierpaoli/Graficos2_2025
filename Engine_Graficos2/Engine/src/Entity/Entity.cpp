#include "Entity.h"
#include "RendererSingleton.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TransformMatrix {
	glm::mat4 matrix;
	explicit operator glm::mat4() const { return matrix; }
	glm::mat4 asGlmMatrix() const { return matrix; }
};

Entity::Entity()
{
	vBuffer = new unsigned int;
	iBuffer = new unsigned int;
	translation = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };
	parent = nullptr;
	UpdateModel(false);
}

Entity::~Entity()
{
	RendererSingleton::GetRenderer()->DeleteModel(modelId);
	delete vBuffer;
	delete iBuffer;

	for (Entity* child : children) {
		delete child;
	}
	children.clear();
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
	scale.x *= glm::max(0.0f, x);
	scale.y *= glm::max(0.0f, y);
	scale.z *= glm::max(0.0f, z);
	UpdateModel(true);
}

Vector3 Entity::GetTranslation() const
{
	return translation;
}

Vector3 Entity::GetRotation() const
{
	return rotation;
}

Vector3 Entity::GetScale() const
{
	return scale;
}

unsigned int Entity::GetModelId() const
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

	for (Entity* child : children) {
		child->UpdateModel(isModelCreated);
	}
}

glm::mat4 Entity::GetTransformMatrix() const
{
	glm::mat4 localMatrix = CalculateLocalTransform();

	if (parent) {
		return parent->GetTransformMatrix() * localMatrix;
	}

	return localMatrix;
}

glm::mat4 Entity::CalculateLocalTransform() const
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

void Entity::Render(Camera* camera, const std::vector<DirectionalLight>& dirLights, const std::vector<PointLight>& pointLights, const std::vector<SpotLight>& spotLights)
{
}

void Entity::SetName(const std::string& name) {
	this->name = name;
}

const std::string& Entity::GetName() const {
	return name;
}

void Entity::AddChild(Entity* child) {
	children.push_back(child);
	child->parent = this;
}

Entity* Entity::GetParent() const {
	return parent;
}

const std::vector<Entity*>& Entity::GetChildren() const {
	return children;
}

Entity* Entity::FindChildByName(const std::string& name, bool recursive) {
	for (Entity* child : children) {
		if (child->GetName() == name) {
			return child;
		}

		if (recursive) {
			Entity* found = child->FindChildByName(name, true);
			if (found) {
				return found;
			}
		}
	}
	return nullptr;
}
