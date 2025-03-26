#pragma once
#include <string>
#include "Entity/Entity2D/Entity2D.h"
#include "Exports.h"
#include "Animation/Animation.h"
#include "Frame/Frame.h"

class GraficosEngine_API Sprite : public Entity2D
{
private:
	unsigned int mRendererID;
	unsigned int imageID;
	std::string mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth;
	int mHeight;
	int mBPP;
	float vertices[4][4];
	std::vector<Animation*>* animations;
	unsigned int spriteQty;

	void ChangeSprite(Coord coord);
	void Bind(unsigned int slot = 0) const;
	void Unbind();

public:
	Sprite(const std::string& path);
	Sprite(const std::string& path, float vertexCol[4][4]);
	Sprite(const std::string& path, int spriteQuantity, Frame firstFrame);
	~Sprite();

	unsigned int GetImageID();
	inline int GetSpriteQty() const { return spriteQty; }
	inline int GetImgWidth() const { return mWidth; }
	inline int GetImgHeight() const { return mHeight; }
	inline int GetWidth()const { return mWidth / spriteQty; }
	inline int GetHeight()const { return mHeight; }

	void AddAnimation(Animation* _anim);
	void UpdateFrame(int frameIndex);

	void Draw();
};
