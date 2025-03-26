#include "Game.h"

int main()
{
	Game* game = new Game();
	
	game->Init();
	game->BaseGame::Loop();
	game->DeInit();

	delete game;

	return 0;
}