#include "GamePCH.h"
#include "Game.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::unique_ptr<Game> pGame = std::make_unique<Game>();
	pGame->RunGame();

	return EXIT_SUCCESS;
}