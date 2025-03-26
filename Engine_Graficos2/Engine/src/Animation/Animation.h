#pragma once
#include <vector>
#include "Vector2.h"
#include "Frame/Frame.h"

struct Coord
{
	float x1;
	float y1;
	float x2;
	float y2;
};

class GraficosEngine_API Animation
{
private:
	std::vector<Coord> uCoords; //use only left U and right U
	float timer;
	float length;
	int currentFrame;

public:
	Animation(float animLength, unsigned int textureWidth, unsigned int textureHeight, std::vector<Frame> frameData);
	~Animation();
	void Update();
	void AddFrame(Coord _uCoords);
	void SetDuration(float _length);
	Coord GetCurrentFrame();
	Coord GetFrame(int frame);
};