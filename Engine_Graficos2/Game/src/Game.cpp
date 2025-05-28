#include "Game.h"
#include <iostream>
#include <../libs/glfw/include/GLFW/glfw3.h>
#include <VectorUtils.h>

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

	floor = new Entity3D();
	floor->SetMesh(new CubeMesh());
	floor->Translate(0.0f, -3.0f, 5.0f);
	floor->Scale(50.0f, 2.0f, 50.0f);
	
	/*for (int i = 0; i < 6; i++) 
	{
		cubeFaces[i] = new Sprite("res/Solid_red.png", 1, Frame(0, 200, 0, 200));
	}*/

	cube = new Entity3D();
	cube->SetMesh(new CubeMesh());
	cube->Translate(0.0f, 0.0f, 5.0f);
	cube->Scale(1.0f, 1.0f, 1.0f);

	camera = new Camera(CameraMode::ThirdPerson, 5.0f, 0.2f);

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
	redLight.intensity = 100.0f;                       
	redLight.constant = 1.0f;                        
	redLight.linear = 0.09f;
	redLight.quadratic = 0.032f;

	pointLights.push_back(redLight);
	
	PointLight brownLight;
	brownLight.position = glm::vec3(5.0f, 3.0f, 5.0f); 
	brownLight.color = glm::vec3(0.36f, 0.25f, 0.20f);
	brownLight.intensity = 100.0f;                       
	brownLight.constant = 1.0f;                         
	brownLight.linear = 0.09f;
	brownLight.quadratic = 0.032f;

	pointLights.push_back(brownLight);

	DirectionalLight sunLight;
	sunLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	sunLight.color = glm::vec3(1.0f);
	sunLight.intensity = 5000.0f;
	directionalLights.push_back(sunLight);

	SpotLight spot;
	spot.position = glm::vec3(0.0f, 4.0f, 3.0f);
	spot.direction = glm::vec3(0.0f, -1.0f, -1.0f);  // Pointing down and center
	spot.color = glm::vec3(0.0f, 1.0f, 0.0f);
	spot.cutOff = 15.0f;
	spot.outerCutOff = 25.0f;
	spot.constant = 1.0f;
	spot.linear = 0.09f;
	spot.quadratic = 0.032f;
	spot.intensity = 100.0f;
	spotLights.push_back(spot);


	SpotLight spot2;
	spot2.position = glm::vec3(0.0f, 4.0f, 10.0f);
	spot2.direction = glm::vec3(0.0f, -1.0f, -1.0f);  
	spot2.color = glm::vec3(0.0f, 0.0f, 1.0f);
	spot2.cutOff = 15.0f;
	spot2.outerCutOff = 25.0f;
	spot2.constant = 1.0f;
	spot2.linear = 0.09f;
	spot2.quadratic = 0.032f;
	spot2.intensity = 100.0f;
	spotLights.push_back(spot2);


	goldMaterial = Material(
		glm::vec3(1.0f),   
		glm::vec3(0.75164f, 0.60648f, 0.22648f),  
		glm::vec3(0.628281f, 0.555802f, 0.366065f), 
		51.2f,
		"res/gold.jpg"
	);

	defaultDiffuse = Material(
		glm::vec3(0.5f, 0.5f, 0.5f),  
		glm::vec3(0.8f, 0.8f, 0.8f),  
		glm::vec3(0.0f, 0.0f, 0.0f),  
		1.0f                          
	);

	cube->SetMaterial(goldMaterial);
	floor->SetMaterial(defaultDiffuse);

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
	
	for (int i = 0; i < 6; i++) {
		if (cubeFaces[i] != nullptr) {
			delete cubeFaces[i];
			cubeFaces[i] = nullptr;
		}
	}
	
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

		if (camera->GetMode() == CameraMode::FirstPerson) {
			camera->SetPitch(0.0f);
			glm::vec3 direction;
			direction.x = cos(glm::radians(camera->GetYaw())) * cos(glm::radians(camera->GetPitch()));
			direction.y = sin(glm::radians(camera->GetPitch()));
			direction.z = sin(glm::radians(camera->GetYaw())) * cos(glm::radians(camera->GetPitch()));
			direction = glm::normalize(direction);
			camera->SetLookTarget(camera->GetPosition() + direction);
		}
		else {
			camera->SetPitch(15.0f);
		}
	}
}

void Game::UpdatePlayer()
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
		cube->Translate(movement.x, movement.y, movement.z);
	}

	// Rotation
	if (glm::length(rotationVectorPlayer1) > 0.01f) {
		float rotationAmount = defaultRotation * time->GetDeltaTime();
		cube->RotateX(rotationVectorPlayer1.x * rotationAmount);
		cube->RotateY(rotationVectorPlayer1.y * rotationAmount);
		cube->RotateZ(rotationVectorPlayer1.z * rotationAmount);
	}

	// Scale
	if (scaleVectorPlayer1 != 0) {
		float scaleAmount = scaleVectorPlayer1 * defaultScale.x * time->GetDeltaTime();
		cube->Scale(scaleAmount, scaleAmount, scaleAmount);
	}

	cube->UpdateModel(true);
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
			camera->FollowTarget(cube->GetTranslation(), deltaX, deltaY, true);
		}
		else
		{
			camera->FollowTarget(cube->GetTranslation(), 0, 0, false);
		}
	}
	else
	{
		if (rightHeld)
			camera->UpdateFirstPersonView(GetMouseDeltaX(), GetMouseDeltaY());

		camera->SetLookTarget(camera->GetPosition() + camera->CalculateDirection());

		camera->SetPosition(ToGLM(cube->GetTranslation()) + glm::vec3(0, 1.5f, 0));

		camera->SetPitch(glm::clamp(camera->GetPitch(), -89.0f, 89.0f));
	}
}

void Game::UpdateScene()
{
	//static_cast<Sprite*>(cartel)->UpdateFrame(0);
}

void Game::RenderScene()
{
	glm::mat4 view = camera->GetViewMatrix();

	Window* myWindow = static_cast<Window*>(window);

	cube->GetMesh()->Render(camera, cube->GetMaterial(), directionalLights, pointLights, spotLights);
	floor->GetMesh()->Render(camera, floor->GetMaterial(), directionalLights, pointLights, spotLights);

}