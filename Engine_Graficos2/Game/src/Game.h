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

class Game : public BaseGame
{
private:

	Camera* camera;

	Entity3D* floor;
	Entity3D* cube;
	Sprite* cubeFaces[6];
	std::vector<Entity3D*> entities;

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

	bool isMovingForward;
	bool isMovingBackward;
	bool isMovingLeft;
	bool isMovingRight;	
	bool isMovingNear;	
	bool isMovingFurther;	

	std::vector<Frame> sonicIdleFrames = std::vector<Frame>();
	std::vector<Frame> sonicRunFrames = std::vector<Frame>();
	std::vector<Frame> cartelFrames = std::vector<Frame>();

	void checkCollisions(Entity2D* player1, Entity2D* player2);

	void UpdateInput();
	void UpdatePlayer();
	void UpdateCamera();
	void UpdateScene();
	void RenderScene();
	void LoadModel(
		const std::string& path,
		const Material& material,
		std::vector<Entity3D*>& entities,
		const glm::vec3& position = glm::vec3(1.0f),
		const glm::vec3& scale = glm::vec3(0.0f)
	);

public:
	Game();
	~Game();

	void Init() override;
	void Update() override;
	void DeInit() override;
};
