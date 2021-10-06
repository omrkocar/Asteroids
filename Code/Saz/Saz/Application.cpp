#include "SazPCH.h"
#include "Application.h"

#include "Saz/EntityWorld.h"
#include "Saz/InputComponent.h"
#include "Saz/InputSystem.h"
#include "Saz/RenderSystem.h"
#include "Saz/ResourceManager.h"
#include "Saz/SFML/Window.h"
#include "Saz/SpriteComponent.h"
#include "Saz/TagComponent.h"
#include "Saz/TransformComponent.h"

#include <GLFW/glfw3.h>

namespace Saz
{
	Application* Application::s_Instance = nullptr;

	const ecs::EntityWorld& Application::GetWorld()
	{
		return m_EntityWorld;
	}

	Application::Application()
	{
		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		spd::Log::Init();
		m_ImGuiLog = new imgui::Log();

		// #todo Create all textures with a single call here.
		m_pResourceManager = new Saz::ResourceManager();
		m_pResourceManager->LoadTexture("Ship.png");
		
		{
			Saz::WindowProps windowProps ;
			windowProps.m_Title = "SFML Window";
			windowProps.m_Size = ivec2(1280, 720);
			m_SFMLWindow = new sfml::Window(windowProps);
		}
	}

	Application::~Application()
	{
		delete m_SFMLWindow;
		delete m_pResourceManager;
		delete m_ImGuiLog;
		glfwTerminate();
	}

	void Application::Init()
	{
		m_SFMLWindow->Init();
		m_EntityWorld.Init();
	}

	void Application::Register()
	{
		m_EntityWorld.RegisterComponent<SpriteComponent>();
		m_EntityWorld.RegisterComponent<TagComponent>();
		m_EntityWorld.RegisterComponent<TransformComponent>();
		m_EntityWorld.RegisterComponent<Input::InputComponent>();

		m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_SFMLWindow);
		m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_SFMLWindow);
	}

	void Application::Destroy()
	{
		m_SFMLWindow->Destroy();
		m_EntityWorld.Destroy();
	}

	void Application::Update()
	{
		m_EntityWorld.Update();
		m_ImGuiLog->Update();
	}

	void Application::Run(int argc, char* argv[])
	{
		Register();
		Init();

		while (m_Running)
		{
			m_SFMLWindow->Update();
			if (m_SFMLWindow->ShouldClose())
				break;

			Update();

			m_SFMLWindow->Render();
		}
	}
}