#include "InputManager.h"

InputManager::InputManager(GLFWwindow* window)
{
	this->window = window;
}

InputManager::~InputManager()
{
}

bool InputManager::IsKeyPressed(unsigned int keyCode)
{
	return (glfwGetKey(window, keyCode) == GLFW_PRESS);
}
