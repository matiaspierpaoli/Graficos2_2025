#pragma once
#include "Exports.h"
#include "glfw/include/GLFW/glfw3.h"

class GraficosEngine_API InputManager
{
private:
	GLFWwindow* window;

public:
	InputManager(GLFWwindow* window);
	~InputManager();

	bool IsKeyPressed(unsigned int keyCode);
};