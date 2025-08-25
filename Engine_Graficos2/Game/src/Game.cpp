#include "Game.h"
#include <iostream>
#include <../libs/glfw/include/GLFW/glfw3.h>
#include <VectorUtils.h>
#include <Modelimporter/ModelImporter.h>

#pragma region keyCodes

//Traslate
#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68
#define KEY_Q 81
#define KEY_E 69

//Rotate
#define KEY_R 82
#define KEY_T 84
#define KEY_F 70
#define KEY_G 71
#define KEY_C 67
#define KEY_V 86

//Rotate
#define KEY_Z 90
#define KEY_X 88

#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_U 85
#define KEY_O 79
#define KEY_N 78
#define KEY_M 77

#pragma endregion

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init()
{
	OnStart(1024, 768, "Game");

	traslateX = 0.f;
	traslateY = 0.f;
	traslateZ = 0.f;
	
	defaultTranslation.x = 10.0f;
	defaultTranslation.y = 10.0f; 
	defaultTranslation.z = 10.0f; 

	defaultRotation = 100.0f;

	defaultScale.x = 1.0f;
	defaultScale.y = 1.0f;

	camera = new Camera(CameraMode::ThirdPerson, 7.5f, 0.2f);

	directionalLights = std::vector<DirectionalLight>();
	pointLights = std::vector<PointLight>();
	spotLights = std::vector<SpotLight>();

	PointLight redLight;
	redLight.position = glm::vec3(0.0f, 1.0f, -13.0f); 
	redLight.color = glm::vec3(1.0f, 0.0f, 0.0f);    
	redLight.intensity = 1.0f;                       
	redLight.constant = 1.0f;                        
	redLight.linear = 0.09f;
	redLight.quadratic = 0.032f;

	pointLights.push_back(redLight);
	
	PointLight violetLight;
	violetLight.position = glm::vec3(15.0f, 1.0f, -13.0f);
	violetLight.color = glm::vec3(0.7f, 0.0f, 1.0f);
	violetLight.intensity = 0.7f;
	violetLight.constant = 1.0f;
	violetLight.linear = 0.09f;
	violetLight.quadratic = 0.032f;

	pointLights.push_back(violetLight);

	DirectionalLight sunLight;
	sunLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	sunLight.color = glm::vec3(1.0f, 0.95f, 0.9f);
	sunLight.intensity = 0.8f;
	directionalLights.push_back(sunLight);

	SpotLight spot; // Blue
	spot.position = glm::vec3(0.0f, 4.0f, 3.0f);
	spot.direction = glm::vec3(0.0f, -1.0f, -1.0f);
	spot.color = glm::vec3(0.0f, 1.0f, 0.0f);
	spot.cutOff = 15.0f;
	spot.outerCutOff = 25.0f;
	spot.constant = 1.0f;
	spot.linear = 0.09f;
	spot.quadratic = 0.032f;
	spot.intensity = 1.0f;
	spotLights.push_back(spot);


	SpotLight spot2; // Green
	spot2.position = glm::vec3(0.0f, 4.0f, 10.0f);
	spot2.direction = glm::vec3(0.0f, -1.0f, -1.0f);  
	spot2.color = glm::vec3(0.0f, 0.0f, 1.0f);
	spot2.cutOff = 15.0f;
	spot2.outerCutOff = 25.0f;
	spot2.constant = 1.0f;
	spot2.linear = 0.09f;
	spot2.quadratic = 0.032f;
	spot2.intensity = 1.0f;
	spotLights.push_back(spot2);

	Material floorMat(
		glm::vec3(0.1f),                     // ambient
		glm::vec3(0.8f),                     // diffuse tint
		glm::vec3(0.2f),                     // specular
		32.0f,                               // shininess
		""
	);
	
	floor = new Entity3D();
	floor->AddMesh(new CubeMesh());
	floor->SetMaterial(floorMat);
	floor->Translate(0, -3, 5);
	floor->Scale(50, 2, 50);
	floor->SetStatic(true);
	entities.push_back(floor);

	wall = new Entity3D();
	wall->AddMesh(new CubeMesh());
	wall->SetMaterial(floorMat);
	wall->Translate(0, 5, -20);
	wall->RotateX(90);
	wall->Scale(50, 2, 20);
	wall->SetStatic(true);
	entities.push_back(wall);

	if (auto* ballRoot = LoadModel("res/models/football/Ball.obj",
		{ -5, 0, -5 }, { 1,1,1 }, nullptr)) {
		std::vector<Mesh*> mlist;
		CollectMeshes(ballRoot, mlist);

		Material ballWhite(glm::vec3(0.1f), glm::vec3(1.0f),
			glm::vec3(0.5f), 32.0f,
			"res/models/football/Ball_White_s_BaseColor.png");
		Material ballBlack(glm::vec3(0.1f), glm::vec3(1.0f),
			glm::vec3(0.5f), 32.0f,
			"res/models/football/Ball_Black_s_BaseColor.png");

		if (mlist.size() >= 2) {
			mlist[0]->SetMaterial(ballWhite);
			mlist[1]->SetMaterial(ballBlack);
		}
		else {
			if (!mlist.empty()) mlist[0]->SetMaterial(ballWhite);
		}

		entities.push_back(ballRoot);
	}

	Material backpackMat(
		glm::vec3(0.2f),
		glm::vec3(1.0f),
		glm::vec3(0.5f),
		32.0f,
		"res/models/backpack/1001_albedo.jpg"
	);

	if (auto* backpackRoot = LoadModel("res/models/backpack/backpack.fbx",
		{ 0, 3, 0 }, { 0.01f, 0.01f, 0.01f },
		&backpackMat)) {
		entities.push_back(backpackRoot);
	}

	Material goldMat(
		glm::vec3(1.0f),
		glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f),
		51.2f,
		"res/gold.jpg"
	);
	if (auto* suzRoot = LoadModel("res/models/suzanne.obj",
		{ 0.0f, 2.0f, -5.0f }, { 0.5f, 0.5f, 0.5f }, &goldMat)) {
		entities.push_back(suzRoot);
	}
	
	Material tankMat(
		glm::vec3(0.1f),
		glm::vec3(1.0f),
		glm::vec3(0.5f),
		32.0f,             
		"res/models/tank/uv_albedo.jpg" 
	);

	if (auto* tr = LoadModel("res/models/tank/tank.fbx",
		{ 0.0f, 0.0f, -10.0f }, { 0.03f, 0.03f, 0.03f }, &tankMat))
	{
		tankRoot = tr;
		entities.push_back(tr);

		tankTree = {};
		tankTree.root = tankRoot;
		IndexTree(tankRoot, tankTree);

		// Tank objects paths -> usefull for assigning entity3d objects by hierarchy name
		for (auto& kv : tankTree.byPath) std::cout << kv.first << "\n";

		tankTurret = FindByName(tankTree, "Turret");
		wheelsLeft = FindByName(tankTree, "Wheels_Left");
		wheelsRight = FindByName(tankTree, "Wheels_Right");
	}

	selectedEntity = entities.size() - 1;
}

void Game::DeInit()
{
	if (floor != nullptr)
	{
		delete floor;
		floor = nullptr;
	}
	
	if (wall != nullptr)
	{
		delete wall;
		wall = nullptr;
	}
	
	pointLights.clear();
	directionalLights.clear();
	spotLights.clear();

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] != nullptr)
			entities[i] = nullptr;
	}

	entities.clear();
	
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
}

void Game::Update()
{
	system("cls");

	UpdateInput();
	UpdatePlayer();
	UpdateCamera();
	UpdateScene();
	RenderScene();
}

void Game::UpdateInput()
{
	glm::vec3 rawInput = { 0, 0, 0 };
	scaleVectorPlayer1 = 0;
	glm::vec3 rotationInput = { 0, 0, 0 };

	// Movement (WASD + QE)
	if (IsKeyPressed(GLFW_KEY_W)) rawInput.y += 1;
	if (IsKeyPressed(GLFW_KEY_S)) rawInput.y -= 1;
	if (IsKeyPressed(GLFW_KEY_D)) rawInput.x += 1;
	if (IsKeyPressed(GLFW_KEY_A)) rawInput.x -= 1;
	if (IsKeyPressed(GLFW_KEY_E)) rawInput.z += 1; // Up
	if (IsKeyPressed(GLFW_KEY_Q)) rawInput.z -= 1; // Down

	// Rotation (RT/FG/CV)
	if (IsKeyPressed(GLFW_KEY_R)) rotationInput.x += 1; // Rotate X+
	if (IsKeyPressed(GLFW_KEY_T)) rotationInput.x -= 1; // Rotate X-
	if (IsKeyPressed(GLFW_KEY_F)) rotationInput.y += 1; // Rotate Y+
	if (IsKeyPressed(GLFW_KEY_G)) rotationInput.y -= 1; // Rotate Y-
	if (IsKeyPressed(GLFW_KEY_C)) rotationInput.z += 1; // Rotate Z+
	if (IsKeyPressed(GLFW_KEY_V)) rotationInput.z -= 1; // Rotate Z-

	// Scale (Z/X)
	if (IsKeyPressed(GLFW_KEY_Z)) scaleVectorPlayer1 += 1; // Bigger
	if (IsKeyPressed(GLFW_KEY_X)) scaleVectorPlayer1 -= 1; // Smaller

	moveVectorPlayer1 = (glm::length(rawInput) > 0.01f) ? glm::normalize(rawInput) : glm::vec3(0, 0, 0);
	rotationVectorPlayer1 = (glm::length(rotationInput) > 0.01f) ? glm::normalize(rotationInput) : glm::vec3(0, 0, 0);

	// Camera mode (TAB)
	if (IsKeyJustReleased(GLFW_KEY_TAB)) {
		camera->ToggleMode();
		if (camera->GetMode() == CameraMode::ThirdPerson) {
			// back to offset TP
			camera->FollowTarget(entities[selectedEntity]->GetTranslation(), 0, 0, false);
		}
		else {
			// Upon changing to First Person, places camera above head
			glm::vec3 headPos = ToGLM(entities[selectedEntity]->GetTranslation()) + glm::vec3(0, 1.5f, 0);
			camera->SetPosition(headPos);
			glm::vec3 forward = camera->CalculateDirection();
			camera->SetLookTarget(headPos + forward);
			camera->SetPitch(0.0f);
		}
	}

	if (IsKeyJustReleased(GLFW_KEY_N)) {
		selectedEntity++;
		if (selectedEntity >= entities.size())
			selectedEntity = 2;
		std::cout << "Entidad activa: " << selectedEntity << "\n";
	}
	if (IsKeyJustReleased(GLFW_KEY_B)) {
		if (selectedEntity <= 2)
			selectedEntity = entities.size() - 1;
		else
			selectedEntity--;
		std::cout << "Entidad activa: " << selectedEntity << "\n";
	}

	turretYawInput = 0.0f;
	leftSlideInput = 0.0f;
	rightSlideInput = 0.0f;

	if (IsKeyPressed(GLFW_KEY_J)) turretYawInput -= 1.0f;
	if (IsKeyPressed(GLFW_KEY_L)) turretYawInput += 1.0f;

	if (IsKeyPressed(GLFW_KEY_U)) leftSlideInput += 1.0f;
	if (IsKeyPressed(GLFW_KEY_O)) leftSlideInput -= 1.0f;

	if (IsKeyPressed(GLFW_KEY_I)) rightSlideInput += 1.0f;
	if (IsKeyPressed(GLFW_KEY_K)) rightSlideInput -= 1.0f;
}

void Game::UpdatePlayer()
{
	if (camera->GetMode() == CameraMode::ThirdPerson)
	{
		// Movement
		if (glm::length(moveVectorPlayer1) > 0.01f) {
			glm::vec3 forward = camera->GetForward();
			forward.y = 0;
			forward = glm::normalize(forward);

			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
			glm::vec3 up = glm::vec3(0, 1, 0);

			glm::vec3 moveDir = moveVectorPlayer1.x * right + moveVectorPlayer1.y * forward + moveVectorPlayer1.z * up;

			if (glm::length(moveDir) > 0.01f)
				moveDir = glm::normalize(moveDir);

			glm::vec3 movement = moveDir * defaultTranslation.x * time->GetDeltaTime();
			entities[selectedEntity]->Translate(movement.x, movement.y, movement.z);
		}

		// Rotation
		if (glm::length(rotationVectorPlayer1) > 0.01f) {
			float rotationAmount = defaultRotation * time->GetDeltaTime();
			entities[selectedEntity]->RotateX(rotationVectorPlayer1.x * rotationAmount);
			entities[selectedEntity]->RotateY(rotationVectorPlayer1.y * rotationAmount);
			entities[selectedEntity]->RotateZ(rotationVectorPlayer1.z * rotationAmount);
		}

		// Scale
		if (scaleVectorPlayer1 != 0) {
			float scaleAmount = 1.0f + scaleVectorPlayer1 * defaultScale.x * time->GetDeltaTime();
			entities[selectedEntity]->Scale(scaleAmount, scaleAmount, scaleAmount);
		}
	}
	else // FirstPerson
	{
		glm::vec3 camPos = camera->GetPosition();
		glm::vec3 forward = camera->CalculateDirection();        // hacia donde mira
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::vec3 movement =
			moveVectorPlayer1.x * right * defaultTranslation.x * time->GetDeltaTime() +
			moveVectorPlayer1.y * forward * defaultTranslation.y * time->GetDeltaTime() +
			moveVectorPlayer1.z * up * defaultTranslation.z * time->GetDeltaTime();

		camera->SetPosition(camPos + movement);
	}

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->UpdateModel(true);
	}
}

void Game::UpdateCamera()
{
	bool rightHeld = IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT);
	bool altHeld = IsKeyPressed(GLFW_KEY_LEFT_ALT);
	bool leftHeld = IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);

	if (camera->IsThirdPerson())
	{
		if (altHeld && leftHeld)
		{
			float deltaX = GetMouseDeltaX();
			float deltaY = GetMouseDeltaY();
			camera->FollowTarget(entities[selectedEntity]->GetTranslation(), deltaX, deltaY, true);
		}
		else
		{
			camera->FollowTarget(entities[selectedEntity]->GetTranslation(), 0, 0, false);
		}
	}
	else
	{
		if (rightHeld)
			camera->UpdateFirstPersonView(GetMouseDeltaX(), GetMouseDeltaY());

		glm::vec3 dir = camera->CalculateDirection();
		camera->SetLookTarget(camera->GetPosition() + dir);
	}
}

void Game::UpdateScene()
{
	const float dt = time->GetDeltaTime();

	if (tankTurret && std::abs(turretYawInput) > 0.001f)
	{
		const float yawSpeed = 60.0f; 
		tankTurret->RotateZ(turretYawInput * yawSpeed * dt);
	}

	const float slideSpeed = 0.5f;
	const float maxOffset = 0.15f;

	if (wheelsLeft && std::abs(leftSlideInput) > 0.001f)
	{
		float d = leftSlideInput * slideSpeed * dt;
		leftOffset = glm::clamp(leftOffset + d, -maxOffset, maxOffset);
		float applied = leftOffset - wheelsLeft->GetTranslation().x; 
		wheelsLeft->Translate(d, 0.0f, 0.0f);
	}

	if (wheelsRight && std::abs(rightSlideInput) > 0.001f)
	{
		float d = rightSlideInput * slideSpeed * dt;
		rightOffset = glm::clamp(rightOffset + d, -maxOffset, maxOffset);
		wheelsRight->Translate(d, 0.0f, 0.0f);
	}

	for (size_t i = 0; i < entities.size(); ++i)
	{
		auto* a = dynamic_cast<Entity3D*>(entities[i]);
		if (!a) continue;

		for (size_t j = i + 1; j < entities.size(); ++j)
		{
			auto* b = dynamic_cast<Entity3D*>(entities[j]);
			if (!b) continue;

			if (CollisionManager::IntersectAABB3D(a, b))
			{
				CollisionManager::ResolveSimple(a, b);
			}
		}
	}
}

void Game::RenderScene()
{
	for (auto& entity : entities) {
		entity->Render(camera, directionalLights, pointLights, spotLights);
	}
}

Entity3D* Game::LoadModel(
	const std::string& path,
	const glm::vec3& pos,
	const glm::vec3& scl,
	const Material* overrideMat = nullptr)
{
	Entity3D* root = ModelImporter::LoadModelWithHierarchy(path);
	if (!root) {
		std::cerr << "ERROR: no se pudo cargar " << path << "\n";
		return nullptr;
	}
	root->Translate(pos.x, pos.y, pos.z);
	root->Scale(scl.x, scl.y, scl.z);

	if (overrideMat) {
		ApplyMaterialRecursive(root, *overrideMat);
	}
	return root;
}

void Game::ApplyMaterialRecursive(Entity3D* node, const Material& mat) {
	if (!node) return;
	node->SetMaterial(mat);
	node->SetMaterialToMeshes();

	for (auto* child : node->GetChildren()) {
		if (auto* e3d = dynamic_cast<Entity3D*>(child)) {
			ApplyMaterialRecursive(e3d, mat);
		}
	}
}

void Game::CollectMeshes(Entity3D* node, std::vector<Mesh*>& out) {
	if (!node) return;

	for (auto* m : node->GetMeshes())
		out.push_back(m);

	for (auto* child : node->GetChildren()) {
		if (auto* e3d = dynamic_cast<Entity3D*>(child))
			CollectMeshes(e3d, out);
	}
}