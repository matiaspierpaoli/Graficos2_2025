#include "Animation.h"
#include "BaseGame/BaseGame.h"
#include "TimeSingleton.h"

#include <iostream>

Animation::Animation(float animLength, unsigned int textureWidth, unsigned int textureHeight, std::vector<Frame> frameData)
{
	length = animLength;
	currentFrame = 0;
	timer = 0;

	for (unsigned int i = 0; i < frameData.size(); i++)
	{
		//Calculate left and right of frame
		Coord frameCoords;
		frameCoords.x1 = (float)frameData[i].GetLeftX() / textureWidth;
		frameCoords.y1 = (float)(frameData[i].GetTopY()) / textureHeight;

		frameCoords.x2 = (float)frameData[i].GetRightX() / textureWidth;
		frameCoords.y2 = (float)(frameData[i].GetBotY()) / textureHeight;

		//Send frame U coordinates to the vector
		AddFrame(frameCoords);
	}
}

Animation::~Animation()
{
}

void Animation::Update()
{
	timer += TimeSingleton::GetTime()->GetDeltaTime();

	while (timer >= length) {
		timer -= length;
	}

	float frameLength = length / uCoords.size();
	currentFrame = static_cast<int>(timer / frameLength);
}

void Animation::AddFrame(Coord _uCoords)
{
	uCoords.push_back(_uCoords);
}

void Animation::SetDuration(float _length)
{
	length = _length;
}

Coord Animation::GetCurrentFrame()
{
	return uCoords[currentFrame];
}

Coord Animation::GetFrame(int frame)
{
	return uCoords[frame];
}