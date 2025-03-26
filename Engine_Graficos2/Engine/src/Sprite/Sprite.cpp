#include "Sprite.h"
#include "RendererSingleton.h"

Sprite::Sprite(const std::string& path)
{
	mRendererID = 0;
	mFilePath = path;
	mWidth = 0;
	mHeight = 0;
	mBPP = 0;
	imageID = mRendererID - 1;
	animations = new std::vector<Animation*>();
}

Sprite::Sprite(const std::string& path, float vertexCol[4][4]) : mRendererID(0), mFilePath(path), mLocalBuffer(nullptr), mWidth(0), mHeight(0), mBPP(0)
{
	mRendererID = 0;
	mFilePath = path;
	mWidth = 0;
	mHeight = 0;
	mBPP = 0;
	imageID = mRendererID - 1;
	animations = new std::vector<Animation*>();

	RendererSingleton::GetRenderer()->GetNewSprite(mFilePath, &mWidth, &mHeight, &mBPP, &mRendererID);

	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};

	float uvPos[4][2] =
	{
		{0, 0}, //bot left
		{1, 0}, //bot right
		{1, 1}, //top right
		{0, 1}  //top left
	};
	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 6; j++)
		{
			vertices[i][j] = vertexCol[i][j - 2];
		}
		for (unsigned short j = 6; j < 8; j++)
		{			
			vertices[i][j] = uvPos[i][j - 6];
		}
	}

	Bind();

	//*vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	*vBuffer = RendererSingleton::GetRenderer()->GetNewVertexBuffer(vertices, 4 * (2 * sizeof(float) + 4 * sizeof(float) + 2 * sizeof(float)));
	*iBuffer = RendererSingleton::GetRenderer()->GetNewIndexBuffer(indices, 6);

	Unbind();
}

Sprite::Sprite(const std::string& path, int spriteQuantity, Frame firstFrame)
{
	mRendererID = 0;
	mFilePath = path;
	mWidth = 0;
	mHeight = 0;
	mBPP = 0;
	spriteQty = spriteQuantity;
	animations = new std::vector<Animation*>();

	Renderer* tempRenderer = RendererSingleton::GetRenderer();
	tempRenderer->GetNewSprite(mFilePath, &mWidth, &mHeight, &mBPP, &mRendererID);

	imageID = mRendererID - 1;

	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};

	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	Coord frameCoords;
	frameCoords.x1 = (float)firstFrame.GetLeftX() / mWidth;
	frameCoords.y1 = (float)(firstFrame.GetTopY()) / mHeight;

	frameCoords.x2 = (float)firstFrame.GetRightX() / mWidth;
	frameCoords.y2 = (float)(firstFrame.GetBotY()) / mHeight;

	float uvPos[4][2] =
	{
		{frameCoords.x1, frameCoords.y1}, //bot left
		{frameCoords.x2, frameCoords.y1}, //bot right
		{frameCoords.x2, frameCoords.y2}, //top right
		{frameCoords.x1, frameCoords.y2}  //top left
	};
		
	for (unsigned short i = 0; i < 4; i++)
	{
		for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertexPos[i][j];
		}
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}
	

	*vBuffer = tempRenderer->GetNewVertexBuffer(vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));
	*iBuffer = tempRenderer->GetNewIndexBuffer(indices, 6);

	Bind();
}

Sprite::~Sprite()
{
	RendererSingleton::GetRenderer()->DeleteSprite(&mRendererID);

	if (animations->size() > 0)
		delete animations;
}

void Sprite::Bind(unsigned int slot) const
{
	RendererSingleton::GetRenderer()->BindSprite(imageID, mRendererID);
	RendererSingleton::GetRenderer()->SetSprite(imageID);
}

void Sprite::Unbind()
{
	RendererSingleton::GetRenderer()->UnbindSprite();
}

unsigned int Sprite::GetImageID()
{
	return imageID;
}

void Sprite::ChangeSprite(Coord coord)
{
	float vertexPos[4][2] =
	{
		{-1, -1},
		{1, -1},
		{1, 1},
		{-1, 1}
	};

	float uvPos[4][2] =
	{
		{coord.x1, coord.y1}, //bot left
		{coord.x2, coord.y1}, //bot right
		{coord.x2, coord.y2}, //top right
		{coord.x1, coord.y2}  //top left
	};

	//ONLY CHANGES TEX COORD FROM EACH VERTEX
	for (unsigned short i = 0; i < 4; i++)
	{
		/*for (unsigned short j = 0; j < 2; j++)
		{
			vertices[i][j] = vertices[i][j];
		}*/
		for (unsigned short j = 2; j < 4; j++)
		{
			vertices[i][j] = uvPos[i][j - 2];
		}
	}

	RendererSingleton::GetRenderer()->GetNewVertexBuffer(*vBuffer, vertices, 4 * (sizeof(float) * 2 + sizeof(float) * 2));

	Bind();

	//UnBind();
}

void Sprite::AddAnimation(Animation* _anim)
{
	//anim = _anim;
	animations->push_back(_anim);
}

void Sprite::UpdateFrame(int frameIndex)
{
	//Get old coords
	Coord oldCoords = animations->at(frameIndex)->GetCurrentFrame();

	//Update animation timer
	animations->at(frameIndex)->Update();

	//Get new coords
	Coord uCoords = animations->at(frameIndex)->GetCurrentFrame();

	//If frame didn't change, exit
	if (oldCoords.x1 == uCoords.x1) return;

	//If it's in new frame, update sprite
	ChangeSprite(uCoords);
}

void Sprite::Draw()
{
	Bind();
	RendererSingleton::GetRenderer()->Draw(*vBuffer, *iBuffer, modelId);
	Unbind();
}