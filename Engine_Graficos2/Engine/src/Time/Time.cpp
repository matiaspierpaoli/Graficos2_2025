#include "Time.h"

#include <glfw/include/GLFW/glfw3.h>

Time::Time()
{
	lastFrameTime = 0.f;
	deltaTime = 0.f;
}

Time::~Time()
{
}

void Time::Update()
{
	float currentTime = glfwGetTime();
	deltaTime = glfwGetTime() - lastFrameTime;
	lastFrameTime = currentTime;
}

float Time::GetDeltaTime()
{
	return deltaTime;
}