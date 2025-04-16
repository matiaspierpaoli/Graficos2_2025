#pragma once
#include "Exports.h"
#include "glfw/include/GLFW/glfw3.h"
#include <unordered_map>

class GraficosEngine_API InputManager
{
private:
	GLFWwindow* window;
	std::unordered_map<unsigned int, int> keyStates;

	double lastMouseX = 0.0, lastMouseY = 0.0;
	double mouseDeltaX = 0.0, mouseDeltaY = 0.0;

public:
	InputManager(GLFWwindow* window);
	~InputManager();

	void Update(GLFWwindow* window);

	bool IsKeyPressed(unsigned int keyCode);
	bool IsKeyJustReleased(unsigned int keyCode);

	bool IsMouseButtonDown(int button);
	float GetMouseDeltaX() const;
	float GetMouseDeltaY() const;
};