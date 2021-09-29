#include "GamePCH.h"
#include "Core/GameApplication.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::unique_ptr<GameApplication> app = std::make_unique<GameApplication>();
	app->Run();

	return EXIT_SUCCESS;
}