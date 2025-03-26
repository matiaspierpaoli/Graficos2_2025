#pragma once
#include "Exports.h"

class  GraficosEngine_API Frame
{
private:
	float leftX;
	float rightX;
	float topY;
	float botY;

public:
	/// <summary> Frame Constructor.
	/// <para>Left 'x' pixel, Right 'x' pixel, texture height - bottom 'y' pixel, texture height - top 'y' pixel </para>
	/// </summary>
	Frame(int mLeftX, int mRightX, int mBotY, int mTopY) {
		leftX = mLeftX;
		rightX = mRightX;
		botY = mTopY; // Due to OpenGL coordinates
		topY = mBotY; // Due to OpenGL coordinates
	}

	float GetLeftX()  { return leftX; }
	float GetRightX() { return rightX; }
	float GetTopY()   { return topY; }
	float GetBotY()   { return botY; }
};