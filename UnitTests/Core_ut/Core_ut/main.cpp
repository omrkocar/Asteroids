#define CATCH_CONFIG_RUNNER
#include <Catch2/catch.hpp>

int main(int argc, char* argv[])
{
	Catch::Session session;
	session.applyCommandLine(argc, argv);
	return session.run();
}