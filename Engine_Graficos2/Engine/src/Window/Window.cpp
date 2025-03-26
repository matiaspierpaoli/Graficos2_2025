#include "Window.h"
#include <iostream>

#include "glew/include/GL/glew.h"
#include <glfw/include/GLFW/glfw3.h>

Window::Window(float width, float height, const char* programName)
{
	this->width = width;
	this->height = height;
	this->programName = programName;
	CreateWindow();
}

Window::~Window()
{

}


void Window::ProcessWindowEvents()
{
	glfwPollEvents();
}

bool Window::WindowExists()
{
	return window;
}

bool Window::WindowShouldClose()
{
	return glfwWindowShouldClose((GLFWwindow*)window);
}

void Window::CreateWindow()
{
	window = glfwCreateWindow(width, height, programName, NULL, NULL);

	if (!window)
		return;

	glfwMakeContextCurrent((GLFWwindow*)window);

	if (glewInit() != GLEW_OK)
		std::cout << "Glew could not be opened\n";

}

void* Window::GetGLFWPointer()
{
	return window;
}

float Window::GetHeight()
{
	return height;
}

float Window::GetWidth()
{
	return width;
}
