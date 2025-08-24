#pragma once
#include "BaseGame/BaseGame.h"
#include "Entity\Entity2D\Shape\Square\Square.h"
#include "Entity\Entity2D\Shape\Triangle\Triangle.h"
#include "Entity/Entity3D/Entity3D.h"
#include "Mesh/CubeMesh/CubeMesh.h"
#include "Sprite/Sprite.h"
#include "Camera/Camera.h"
#include "Light/Light.h"
#include "Material/Material.h"
#include <RigIndex.h>

class Game : public BaseGame
{
private:

	Camera* camera;

	Entity3D* floor;
	Entity3D* wall;
	Entity3D* cube;
	std::vector<Entity3D*> entities;
	size_t selectedEntity;

	// Tank hierarchy
	ModelTree  tankTree;
	Entity3D* tankRoot = nullptr;
	Entity3D* tankTurret = nullptr;     // gira en yaw
	Entity3D* wheelsLeft = nullptr;     // Base/Wheels_Left
	Entity3D* wheelsRight = nullptr;     // Base/Wheels_Right

	// inputs acumulados por frame
	float turretYawInput = 0.0f;   // -1..1
	float leftSlideInput = 0.0f;   // -1..1 (salir/entrar)
	float rightSlideInput = 0.0f;   // -1..1

	// offsets (para clamp opcional)
	float leftOffset = 0.0f;
	float rightOffset = 0.0f;

	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	Material goldMaterial;
	Material mettalicMaterial;
	Material simpleMaterial;
	Material defaultDiffuse;

	float traslateX;
	float traslateY;
	float traslateZ;
	float scaleX;
	float scaleY;
	Vector3 defaultTranslation;
	float defaultRotation;
	Vector2 defaultScale;
	glm::vec3 moveVectorPlayer1;
	glm::vec3 rotationVectorPlayer1;
	glm::vec3 moveVectorPlayer2;
	float scaleVectorPlayer1;
	float scaleVectorPlayer2;

	std::vector<Frame> sonicIdleFrames = std::vector<Frame>();
	std::vector<Frame> sonicRunFrames = std::vector<Frame>();
	std::vector<Frame> cartelFrames = std::vector<Frame>();

	void checkCollisions(Entity2D* player1, Entity2D* player2);

	Entity3D* LoadModel(const std::string& path,
		const glm::vec3& pos,
		const glm::vec3& scl,
		const Material* overrideMat);

	void ApplyMaterialRecursive(Entity3D* node, const Material& mat);

	void CollectMeshes(Entity3D* node, std::vector<Mesh*>& out);

	void UpdateInput();
	void UpdatePlayer();
	void UpdateCamera();
	void UpdateScene();
	void RenderScene();

public:
	Game();
	~Game();

	void Init() override;
	void Update() override;
	void DeInit() override;
};
