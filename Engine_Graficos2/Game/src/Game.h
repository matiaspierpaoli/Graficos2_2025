#pragma once
#include "BaseGame/BaseGame.h"
#include "Entity\Entity2D\Shape\Square\Square.h"
#include "Entity\Entity2D\Shape\Triangle\Triangle.h"
#include "Sprite/Sprite.h"

class Game : public BaseGame
{
private:

	Entity2D* sonic;
	Entity2D* cartel;
	Entity2D* frontWall;
	Entity2D* sideWall1;
	Entity2D* sideWall2;
	Entity2D* floor;

	Entity2D* square1;
	Entity2D* square2;

	float traslateX;
	float traslateY;
	float traslateZ;
	float scaleX;
	float scaleY;
	Vector3 defaultTranslation;
	float defaultRotation;
	Vector2 defaultScale;
	Vector3 moveVectorPlayer1;
	Vector2 moveVectorPlayer2;
	float scaleVectorPlayer1;
	float scaleVectorPlayer2;

	bool isMovingForward;
	bool isMovingBackward;
	bool isMovingLeft;
	bool isMovingRight;	
	bool isMovingNear;	
	bool isMovingFurther;	

	std::vector<Frame> sonicIdleFrames = std::vector<Frame>();
	std::vector<Frame> sonicRunFrames = std::vector<Frame>();
	std::vector<Frame> cartelFrames = std::vector<Frame>();

	Animation* sonicIdleAnim;
	Animation* sonicRunAnim;
	Animation* cartelAnim;

	void checkCollisions(Entity2D* player1, Entity2D* player2);

public:
	Game();
	~Game();

	void Init() override;
	void Update() override;
	void DeInit() override;
};
