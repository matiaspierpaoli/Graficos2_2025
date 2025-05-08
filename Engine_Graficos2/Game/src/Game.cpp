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

	int sonicSpriteSheetWidth = 830;
	int sonicSpriteSheetHeight = 465;

#pragma region Idle 
	sonicIdleFrames.push_back(Frame(43, 74, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(74, 105, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(105, 137, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(137, 169, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(169, 199, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(199, 230, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(230, 260, sonicSpriteSheetHeight - 68, sonicSpriteSheetHeight - 22));
	sonicIdleFrames.push_back(Frame(22, 54, sonicSpriteSheetHeight - 113, sonicSpriteSheetHeight - 72));
	sonicIdleFrames.push_back(Frame(54, 82, sonicSpriteSheetHeight - 113, sonicSpriteSheetHeight - 72));
	sonicIdleFrames.push_back(Frame(82, 115, sonicSpriteSheetHeight - 113, sonicSpriteSheetHeight - 72));
	sonicIdleFrames.push_back(Frame(115, 150, sonicSpriteSheetHeight - 113, sonicSpriteSheetHeight - 72));
	sonicIdleFrames.push_back(Frame(150, 180, sonicSpriteSheetHeight - 113, sonicSpriteSheetHeight - 72));
#pragma endregion 

	#pragma region Run 
	sonicRunFrames.push_back(Frame(276, 304, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(304, 335, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(335, 377, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(377, 417, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(417, 455, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(455, 490, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(490, 520, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(520, 555, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(555, 593, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(593, 634, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(642, 672, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
	sonicRunFrames.push_back(Frame(672, 705, sonicSpriteSheetHeight - 162, sonicSpriteSheetHeight - 117));
#pragma endregion 


	sonic = new Sprite("res/Sonic_Mania_Sprite_Sheet.png",  12, sonicIdleFrames.at(0));
	//sonic->Scale(-0.8, -0.8);
	//sonic->Translate(512 / 2 - 70, 257 / 2 - 30);

	sonicIdleAnim = new Animation(3, sonicSpriteSheetWidth, sonicSpriteSheetHeight, sonicIdleFrames);
	sonicRunAnim = new Animation(0.8, sonicSpriteSheetWidth, sonicSpriteSheetHeight, sonicRunFrames);
	
	static_cast<Sprite*>(sonic)->AddAnimation(sonicIdleAnim);
	static_cast<Sprite*>(sonic)->AddAnimation(sonicRunAnim);

#pragma endregion Cartel

	cartelFrames.push_back(Frame(130, 181, sonicSpriteSheetHeight - 372, sonicSpriteSheetHeight - 322));
	cartelFrames.push_back(Frame(181, 230, sonicSpriteSheetHeight - 372, sonicSpriteSheetHeight - 322));
	cartelFrames.push_back(Frame(230, 279, sonicSpriteSheetHeight - 372, sonicSpriteSheetHeight - 322));
	cartelFrames.push_back(Frame(279, 328, sonicSpriteSheetHeight - 372, sonicSpriteSheetHeight - 322));
	cartelFrames.push_back(Frame(328, 378, sonicSpriteSheetHeight - 372, sonicSpriteSheetHeight - 322));

	cartel = new Sprite("res/Sonic_Mania_Sprite_Sheet.png", 5, cartelFrames.at(0));
	//cartel->Scale(-0.6, -0.6);
	cartel->Translate(0, 0, -2);
	//cartel->Scale(1.0, 1.0);

	cartelAnim = new Animation(3, sonicSpriteSheetWidth, sonicSpriteSheetHeight, cartelFrames);

	static_cast<Sprite*>(cartel)->AddAnimation(cartelAnim);

#pragma endregion

	frontWall = new Sprite("res/frontWall.png", 1, Frame(0, 370, 0, 186));
	frontWall->Translate(0, 0.3, -2);
	frontWall->Scale(3 , 2.2,0);

	floor = new Sprite("res/floor.png", 1, Frame(0, 1024, 0, 1000));
	floor->Translate(0, -1.1, 0);
	floor->RotateX(-90);
	floor->Scale(2.5, 1.8,0);
	
	sideWall1 = new Sprite("res/sideWalls.png", 1, Frame(0, 800, 0, 600));
	sideWall1->Translate(-1.8, 0.3, 0);
	sideWall1->RotateY(90);
	sideWall1->Scale(2, 1.8,0);
	
	sideWall2 = new Sprite("res/sideWalls.png", 1, Frame(0, 800, 0, 600));
	sideWall2->Translate(1.8, 0.3, 0);
	sideWall2->RotateY(-90);
	sideWall2->Scale(2, 1.8,0);


	//float halfSize = 0.5f;

	//// Front face
	//sonicFaces[0]->Translate(0, 0, halfSize);
	//// Back face
	//sonicFaces[1]->Translate(0, 0, -halfSize);
	//sonicFaces[1]->RotateY(180);

	//// Right face
	//sonicFaces[2]->Translate(halfSize, 0, 0);
	//sonicFaces[2]->RotateY(-90);

	//// Left face
	//sonicFaces[3]->Translate(-halfSize, 0, 0);
	//sonicFaces[3]->RotateY(90);

	//// Up face
	//sonicFaces[4]->Translate(0, halfSize, 0);
	//sonicFaces[4]->RotateX(-90);

	//// Down face
	//sonicFaces[5]->Translate(0, -halfSize, 0);
	//sonicFaces[5]->RotateX(90);
	
	for (int i = 0; i < 6; i++) 
	{
		cubeFaces[i] = new Sprite("res/Solid_red.png", 1, Frame(0, 200, 0, 200));
	}

	//cubeFaces[5] = new Sprite("res/Solid_red.png", 1, Frame(0, 200, 0, 200));

	cube = new Entity3D();
	cube->SetMesh(new CubeMesh(cubeFaces));
	cube->Translate(0.0f, 0.0f, -5.0f);
	cube->Scale(2.0f, 2.0f, 2.0f);

	//float vertexCol1[4][4] =
	//{
	//	 0.0f,  1.0f, 0.0f, 1.0f,
	//	 0.0f,  1.0f, 0.0f, 1.0f,
	//	 0.0f,  1.0f, 0.0f, 1.0f,
	//	 0.0f,  1.0f, 0.0f, 1.0f,
	//};

	//float vertexCol2[4][4] =
	//{
	//	 0.0f,  0.0f, 1.0f, 1.0f,
	//	 0.0f,  0.0f, 1.0f, 1.0f,
	//	 0.0f,  0.0f, 1.0f, 1.0f,
	//	 0.0f,  0.0f, 1.0f, 1.0f,
	//};

	//square1 = new Square(vertexCol1);
	//square1->Scale(100, 100);
	//square1->Translate(200, 257 / 2);

	//square2 = new Square(vertexCol2);
	//square2->Scale(100, 100);
	//square2->Translate(512 - 200, 257 / 2);

	camera = new Camera(CameraMode::ThirdPerson, 5.0f, 0.2f);
	
	bool shouldBeVisible = (camera->GetMode() == CameraMode::ThirdPerson);
	static_cast<Sprite*>(sonic)->SetVisible(shouldBeVisible);

	isMovingForward = false;
	isMovingBackward = false;
	isMovingLeft = false;
	isMovingRight = false;
	isMovingNear = false;
	isMovingFurther = false;
}

void Game::DeInit()
{
	if (sonic != nullptr)
	{
		delete sonic;
		sonic = nullptr;
	}

	if (cartel != nullptr)
	{
		delete cartel;
		cartel = nullptr;
	}

	if (frontWall != nullptr)
	{
		delete frontWall;
		frontWall = nullptr;
	}
	
	if (floor != nullptr)
	{
		delete floor;
		floor = nullptr;
	}
	
	if (sideWall1 != nullptr)
	{
		delete sideWall1;
		sideWall1 = nullptr;
	}
	
	if (sideWall2 != nullptr)
	{
		delete sideWall2;
		sideWall2 = nullptr;
	}

	if (sonicIdleAnim != nullptr)
	{
		delete sonicIdleAnim;
		sonicIdleAnim = nullptr;
	}

	if (sonicRunAnim != nullptr)
	{
		delete sonicIdleAnim;
		sonicIdleAnim = nullptr;
	}

	if (cartelAnim != nullptr)
	{
		delete sonicIdleAnim;
		sonicIdleAnim = nullptr;
	}

	if (square1 != nullptr)
	{
		delete square1;
		square1 = nullptr;
	}

	if (square2 != nullptr)
	{
		delete square2;
		square2 = nullptr;
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
	glm::vec3 rotationInput = { 0, 0, 0 }; // Nuevo: para rotación en 3 ejes

	// Movimiento (WASD + QE)
	if (IsKeyPressed(GLFW_KEY_W)) rawInput.y += 1;
	if (IsKeyPressed(GLFW_KEY_S)) rawInput.y -= 1;
	if (IsKeyPressed(GLFW_KEY_D)) rawInput.x += 1;
	if (IsKeyPressed(GLFW_KEY_A)) rawInput.x -= 1;
	if (IsKeyPressed(GLFW_KEY_E)) rawInput.z += 1; // Subir
	if (IsKeyPressed(GLFW_KEY_Q)) rawInput.z -= 1; // Bajar

	// Rotación (RT/FG/CV)
	if (IsKeyPressed(GLFW_KEY_R)) rotationInput.x += 1; // Rotar X+
	if (IsKeyPressed(GLFW_KEY_T)) rotationInput.x -= 1; // Rotar X-
	if (IsKeyPressed(GLFW_KEY_F)) rotationInput.y += 1; // Rotar Y+
	if (IsKeyPressed(GLFW_KEY_G)) rotationInput.y -= 1; // Rotar Y-
	if (IsKeyPressed(GLFW_KEY_C)) rotationInput.z += 1; // Rotar Z+
	if (IsKeyPressed(GLFW_KEY_V)) rotationInput.z -= 1; // Rotar Z-

	// Escala (Z/X)
	if (IsKeyPressed(GLFW_KEY_Z)) scaleVectorPlayer1 += 1; // Agrandar
	if (IsKeyPressed(GLFW_KEY_X)) scaleVectorPlayer1 -= 1; // Achicar

	moveVectorPlayer1 = (glm::length(rawInput) > 0.01f) ? glm::normalize(rawInput) : glm::vec3(0, 0, 0);
	rotationVectorPlayer1 = (glm::length(rotationInput) > 0.01f) ? glm::normalize(rotationInput) : glm::vec3(0, 0, 0);

	// Cambio de cámara (TAB)
	if (IsKeyJustReleased(GLFW_KEY_TAB)) {
		camera->ToggleMode();
		static_cast<Sprite*>(sonic)->SetVisible(!static_cast<Sprite*>(sonic)->IsVisible());

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
	// Movimiento
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

	// Rotación
	if (glm::length(rotationVectorPlayer1) > 0.01f) {
		float rotationAmount = defaultRotation * time->GetDeltaTime();
		cube->RotateX(rotationVectorPlayer1.x * rotationAmount);
		cube->RotateY(rotationVectorPlayer1.y * rotationAmount);
		cube->RotateZ(rotationVectorPlayer1.z * rotationAmount);
	}

	// Escala
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

	//cube->Render(camera);
	cube->GetMesh()->Render(camera);

	static_cast<Sprite*>(frontWall)->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix(myWindow));
	static_cast<Sprite*>(sideWall1)->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix(myWindow));
	static_cast<Sprite*>(sideWall2)->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix(myWindow));
	static_cast<Sprite*>(floor)->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix(myWindow));
	//static_cast<Sprite*>(sonic)->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix(myWindow));
}