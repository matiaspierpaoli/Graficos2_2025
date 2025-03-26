#pragma once

class Window
{
private:
	void* window;
	int height;
	int width;
	const char* programName;
	void CreateWindow();

public:
	Window(float height, float width, const char* programName);
	~Window();

	bool WindowExists();
	bool WindowShouldClose();
	void ProcessWindowEvents();
	void* GetGLFWPointer();
	float GetHeight();
	float GetWidth();
};
