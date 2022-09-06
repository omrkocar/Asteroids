#include "EnginePCH.h"

#include "Application.h"

int main(int agrc, char* argv[])
{
	auto app = Saz::CreateApplication();
	app->Run();

	delete app;

	return EXIT_SUCCESS;
}