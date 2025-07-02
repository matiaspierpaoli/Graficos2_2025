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
	OnStart(512, 257, "Game");

	traslateX = 0.f;
	traslateY = 0.f;
	traslateZ = 0.f;
	
	defaultTranslation.x = 1.0f;
	defaultTranslation.y = 1.0f; 
	defaultTranslation.z = 1.0f; 

	defaultRotation = 100.0f;

	defaultScale.x = 1.0f;
	defaultScale.y = 1.0f;


	camera = new Camera(CameraMode::ThirdPerson, 7.5f, 0.2f);

	isMovingForward = false;
	isMovingBackward = false;
	isMovingLeft = false;
	isMovingRight = false;
	isMovingNear = false;
	isMovingFurther = false;

	directionalLights = std::vector<DirectionalLight>();
	pointLights = std::vector<PointLight>();
	spotLights = std::vector<SpotLight>();

	PointLight redLight;
	redLight.position = glm::vec3(-5.0f, 3.0f, 5.0f); 
	redLight.color = glm::vec3(1.0f, 0.0f, 0.0f);    
	redLight.intensity = 1.0f;                       
	redLight.constant = 1.0f;                        
	redLight.linear = 0.09f;
	redLight.quadratic = 0.032f;

	pointLights.push_back(redLight);
	
	PointLight violetLight;
	violetLight.position = glm::vec3(10.0f, 3.0f, 5.0f);
	//brownLight.color = glm::vec3(0.36f, 0.25f, 0.20f);
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
	spot.direction = glm::vec3(0.0f, -1.0f, -1.0f);  // Pointing down and center
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
	entities.push_back(floor);

	auto meshes = ModelImporter::LoadModel("res/models/football/Ball.obj");

	Material footballMatBlack(
		glm::vec3(0.1f),                     // ambient
		glm::vec3(1.0f),                     // diffuse tint
		glm::vec3(0.5f),                     // specular
		32.0f,                               // shininess
		"res/models/football/Ball_Black_s_BaseColor.png"// ruta al base color
	);

	Material footballMatWhite(
		glm::vec3(0.1f),                     // ambient
		glm::vec3(1.0f),                     // diffuse tint
		glm::vec3(0.5f),                     // specular
		32.0f,                               // shininess
		"res/models/football/Ball_White_s_BaseColor.png"// ruta al base color
	);

	football = new Entity3D();
	football->Translate(-5, 0.0f, -5);
	football->Scale(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < meshes.size(); ++i) {
		auto* m = meshes[i];
		football->AddMesh(m);
		 if (i == 0)   m->SetMaterial(footballMatWhite);
		 else if (i == 1)        m->SetMaterial(footballMatBlack);
	}

	entities.push_back(football);

	Material backpackMat(
		glm::vec3(0.1f),                     // ambient
		glm::vec3(1.0f),                     // diffuse tint
		glm::vec3(0.5f),                     // specular
		32.0f,                               // shininess
		"res/models/backpack/1001_albedo.jpg"// ruta al base color
	);

	meshes = ModelImporter::LoadModel("res/models/backpack/Survival_BackPack_2.fbx");

	backpack = new Entity3D();
	backpack->Translate(0, 0, 0);
	backpack->Scale(0.01f,0.01f,0.01f);
	backpack->SetMaterial(backpackMat);
	for (auto* m : meshes) {
		backpack->AddMesh(m);
	}
	backpack->SetMaterialToMeshes();
	entities.push_back(backpack);

	meshes = ModelImporter::LoadModel("res/models/jet/jet.glb");

	Material jetMat(
		glm::vec3(0.1f),                     // ambient
		glm::vec3(1.0f),                     // diffuse tint
		glm::vec3(0.5f),                     // specular
		32.0f,                               // shininess
		""// ruta al base color
	);

	jet = new Entity3D();
	jet->Translate(10, -2, 10);
	jet->Scale(2.0f, 2.0f, 2.0f);
	jet->SetMaterial(jetMat);
	for (auto* m : meshes) {
		jet->AddMesh(m);
	}
	jet->SetMaterialToMeshes();
	entities.push_back(jet);
	

	Material goldMat(
		glm::vec3(1.0f),  // ambient
		glm::vec3(0.75164f, 0.60648f, 0.22648f), // diffuse tint
		glm::vec3(0.628281f, 0.555802f, 0.366065f), // specular
		51.2f,                               // shininess
		"res/gold.jpg"
	);

	LoadModel("res/models/suzanne.obj", goldMat, entities, glm::vec3(0.0f, 2.0f, -5.0f), glm::vec3(0.5f));

	/*LoadModel("res/models/suzanne.obj", goldMaterial, entities, glm::vec3(0.0f, 2.0f, -10.0f));
	LoadModel("res/models/model.dae", goldMaterial, entities, glm::vec3(1.0f), glm::vec3(0.1f));
	LoadModel("res/models/Buidling 2_2.fbx", simpleMaterial, entities, glm::vec3(1.0f), glm::vec3(-0.9f));*/

	selectedEntity = 1;
}

void Game::DeInit()
{
	if (floor != nullptr)
	{
		delete floor;
		floor = nullptr;
	}

	if (cube != nullptr)
	{
		delete cube;
		cube = nullptr;
	}

	if (backpack != nullptr)
	{
		delete backpack;
		backpack = nullptr;
	}
	
	if (jet != nullptr)
	{
		delete jet;
		jet = nullptr;
	}
	
	if (football != nullptr)
	{
		delete football;
		football = nullptr;
	}
	
	for (int i = 0; i < 6; i++) {
		if (cubeFaces[i] != nullptr) {
			delete cubeFaces[i];
			cubeFaces[i] = nullptr;
		}
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
			// regresa al offset TP
			camera->FollowTarget(entities[selectedEntity]->GetTranslation(), 0, 0, false);
		}
		else {
			// al cambiar a FP, ubica la cámara en la cabeza
			glm::vec3 headPos = ToGLM(entities[selectedEntity]->GetTranslation()) + glm::vec3(0, 1.5f, 0);
			camera->SetPosition(headPos);
			// y fija el lookTarget hacia delante
			glm::vec3 forward = camera->CalculateDirection();
			camera->SetLookTarget(headPos + forward);
			camera->SetPitch(0.0f); // o el valor que prefieras
		}
	}

	if (IsKeyJustReleased(GLFW_KEY_N)) {
		selectedEntity++;
		if (selectedEntity >= entities.size())
			selectedEntity = 1;
		std::cout << "Entidad activa: " << selectedEntity << "\n";
	}
	if (IsKeyJustReleased(GLFW_KEY_B)) {
		if (selectedEntity <= 1)
			selectedEntity = entities.size() - 1;
		else
			selectedEntity--;
		std::cout << "Entidad activa: " << selectedEntity << "\n";
	}
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
		// 1) Calcula ejes de la cámara
		glm::vec3 camPos = camera->GetPosition();
		glm::vec3 forward = camera->CalculateDirection();        // hacia donde mira
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::vec3(0, 1, 0);

		// 2) Construye el desplazamiento a partir de moveVectorPlayer1
		//    (en UpdateInput rawInput.x = izquierda/derecha, rawInput.y = adelante/atrás, rawInput.z = arriba/abajo)
		glm::vec3 movement =
			moveVectorPlayer1.x * right * defaultTranslation.x * time->GetDeltaTime() +
			moveVectorPlayer1.y * forward * defaultTranslation.y * time->GetDeltaTime() +
			moveVectorPlayer1.z * up * defaultTranslation.z * time->GetDeltaTime();

		// 3) Aplica al camera.position
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

		// Mantenemos el lookTarget a partir de la propia orientación
		glm::vec3 dir = camera->CalculateDirection();
		camera->SetLookTarget(camera->GetPosition() + dir);
	}
}

void Game::UpdateScene()
{

}

void Game::RenderScene()
{
	glm::mat4 view = camera->GetViewMatrix();

	Window* myWindow = static_cast<Window*>(window);

	floor->Render(camera, directionalLights, pointLights, spotLights);

	for (auto& entity : entities) {
		entity->Render(camera, directionalLights, pointLights, spotLights);
	}
}

void Game::LoadModel(
	const std::string& path,
	const Material& baseMat,
	std::vector<Entity3D*>& outEntities,
	const glm::vec3& pos,
	const glm::vec3& scl)
{
	auto meshes = ModelImporter::LoadModel(path);

	Entity3D* model = new Entity3D();
	model->Translate(pos.x, pos.y, pos.z);
	model->Scale(scl.x, scl.y, scl.z);

	for (auto* mesh : meshes) {
		mesh->SetMaterial(baseMat);
		model->AddMesh(mesh);
	}

	outEntities.push_back(model);
}
