#include <glfw/include/GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "BaseGame.h"
#include "Window/Window.h"
#include "RendererSingleton.h"
#include "Entity/Entity2D/Shape/Square/Square.h"
#include "Input/InputManager.h"

BaseGame::BaseGame()
{    
   
}

BaseGame::~BaseGame()
{
    glfwTerminate();
    delete window;
    delete renderer;
    delete inputManager;
    delete collisionManager;
}

void BaseGame::OnStart(float width, float height, const char* programName)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "ENGINE ERROR: GLFW Init failed" << std::endl;
        isRunning = false;
        return;
    }

    window = new Window(width, height, programName);
    Window* tempWindow = (Window*)window;

    renderer = new Renderer(tempWindow);

    GLFWwindow* tempGLFWwindow = ((GLFWwindow*)tempWindow->GetGLFWPointer());
    inputManager = new InputManager(tempGLFWwindow);

    collisionManager = new CollisionManager();

    if (!tempWindow->WindowExists())
    {
        std::cout << "ENGINE ERROR: Window Init failed" << std::endl;
        isRunning = false;
        glfwTerminate();
        return;
    }

    RendererSingleton::SetRenderer(new Renderer(tempWindow));
    renderer = RendererSingleton::GetRenderer();

    time = new Time();
    TimeSingleton::SetTime(time);
}

void BaseGame::Loop()
{   
    Window* tempWindow = (Window*)window;
    Renderer* tempRenderer = (Renderer*)renderer;
    
    //Init(); // Call children methods

    while (IsRunning())
    {
        time->Update();
        /* Render here */
        tempRenderer->ClearScreen();

        Update(); // Call children methods

        /* Swap front and back buffers */
        tempRenderer->SwapWindowBuffers();

        /* Poll for and process events */
        tempWindow->ProcessWindowEvents();
    }   
}

bool BaseGame::IsRunning()
{
    return isRunning && !((Window*)window)->WindowShouldClose();
}

bool BaseGame::IsKeyPressed(unsigned int keyCode)
{
    return ((InputManager*)inputManager)->IsKeyPressed(keyCode);
}