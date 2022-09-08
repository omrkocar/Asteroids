#include "GamePCH.h"

#include "Application.h"
#include "GameLayer.h"

namespace Game
{

	Application::Application()
	{
		PushLayer(new GameLayer());
	}

	Application::~Application()
	{
		Destroy();
	}

	void Application::Init()
	{
		Saz::Application::Init();
	}

	void Application::Destroy()
	{
		Saz::Application::Destroy();
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