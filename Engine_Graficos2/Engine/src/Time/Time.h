#pragma once
#include "Exports.h"

class GraficosEngine_API Time
{
private:
	float lastFrameTime;
	float deltaTime;

public:
	Time();
	~Time();

	void Update();
	float GetDeltaTime();
};