#include "GamePCH.h"

#include "Application.h"

namespace Game
{

	Application::Application()
	{
		
	}

	Application::~Application()
	{
		
	}

	void Application::Init()
	{
		Saz::Application::Init();
	}

	void Application::Shutdown()
	{
		Saz::Application::Shutdown();
	}

	void Application::Register()
	{
		Saz::Application::Register();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		Saz::Application::Update(gameTime);
	}
}

Saz::Application* Saz::CreateApplication()
{
	return new Game::Application();
}