#include "GamePCH.h"
#include "Game/Application.h"

int main(int argc, char* argv[])
{
	SAZ_PROFILE_BEGIN_SESSION("Startup", "SazProfile-Startup.json");
	auto app = Saz::CreateApplication();
	SAZ_PROFILE_END_SESSION();

	SAZ_PROFILE_BEGIN_SESSION("Runtime", "SazProfile-Runtime.json");
	app->Run();
	SAZ_PROFILE_END_SESSION();

	SAZ_PROFILE_BEGIN_SESSION("Shutdown", "SazProfile-Shutdown.json");
	delete app;
	SAZ_PROFILE_END_SESSION();
}