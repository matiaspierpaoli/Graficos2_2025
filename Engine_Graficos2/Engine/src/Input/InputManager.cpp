#include "InputManager.h"

InputManager::InputManager(GLFWwindow* window)
{
	this->window = window;
}

InputManager::~InputManager()
{
}

void InputManager::Update(GLFWwindow* window)
{
	double currentX, currentY;
	glfwGetCursorPos(window, &currentX, &currentY);

	mouseDeltaX = currentX - lastMouseX;
	mouseDeltaY = currentY - lastMouseY;

	lastMouseX = currentX;
	lastMouseY = currentY;
}

bool InputManager::IsKeyPressed(unsigned int keyCode)
{
	return (glfwGetKey(window, keyCode) == GLFW_PRESS);
}

bool InputManager::IsKeyJustReleased(unsigned int keyCode)
{
	int currentState = glfwGetKey(window, keyCode);
	int lastState = keyStates[keyCode];

	keyStates[keyCode] = currentState;

	return lastState == GLFW_PRESS && currentState == GLFW_RELEASE;
}

bool InputManager::IsMouseButtonDown(int button)
{
	return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

float InputManager::GetMouseDeltaX() const
{
	return static_cast<float>(mouseDeltaX);
}

float InputManager::GetMouseDeltaY() const
{
	return static_cast<float>(mouseDeltaY);
}
