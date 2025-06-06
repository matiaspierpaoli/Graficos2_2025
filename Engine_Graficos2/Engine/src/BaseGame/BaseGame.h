#pragma once
#include "Exports.h"
#include "TimeSingleton.h"
#include "Collisions/CollisionManager.h"

class GraficosEngine_API BaseGame
{
private:
	bool isRunning = true;
	void* inputManager;

protected:
	void* renderer;
	void* window;
	Time* time;
	CollisionManager* collisionManager;

public:
	BaseGame();
	~BaseGame();

	void OnStart(float height, float width, const char* programName);

	void Loop();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void DeInit() = 0;
	bool IsRunning();
	bool IsKeyPressed(unsigned int keyCode);
	bool IsKeyJustReleased(unsigned int keyCode);
	bool IsMouseButtonDown(int button);
	float GetMouseDeltaX() const;
	float GetMouseDeltaY() const;
};