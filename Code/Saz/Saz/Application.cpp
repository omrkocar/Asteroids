#include "SazPCH.h"
#include "Application.h"

#include "Saz/CameraComponent.h"
#include "Saz/EntityWorld.h"
#include "Saz/GameTime.h"
#include "Saz/CameraSystem.h"
#include "Saz/InputComponent.h"
#include "Saz/InputSystem.h"
#include "Saz/RenderSystem.h"
#include "Saz/ResourceManager.h"
#include "Saz/LevelComponent.h"
#include "Saz/LevelSystem.h"
#include "Saz/MovementComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/NameComponent.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"
#include "Saz/TransformSystem.h"
#include "Saz/Window.h"



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



		Saz::Log::Init();

		{

			Saz::WindowProps windowProps;
			windowProps.m_Title = "Saz Engine";
			windowProps.m_Size = { static_cast<int>(Screen::width), static_cast<int>(Screen::height) };
			m_Window = new Window(windowProps);
			m_Window->Init();
		}
		
		//m_ImGuiLog = new imgui::Log();

		// #todo Create all textures with a single call here.
		m_pResourceManager = new Saz::ResourceManager();	
		
	}

	Application::~Application()
	{
		delete m_Window;
		delete m_ImGuiLog;
		delete m_pResourceManager;
	}

	void Application::Init()
	{
		//m_Window->Init();
		m_EntityWorld.Init();
	}

	void Application::PostInit()
	{
		m_Window->PostInit();
		m_EntityWorld.PostInit();
	}

	void Application::Register()
	{
		m_EntityWorld.RegisterComponent<component::InputComponent>();
		m_EntityWorld.RegisterComponent<component::LevelComponent>();
		m_EntityWorld.RegisterComponent<component::MovementComponent>();
		m_EntityWorld.RegisterComponent<component::NameComponent>();
		m_EntityWorld.RegisterComponent<component::RenderComponent>();
		m_EntityWorld.RegisterComponent<component::TransformComponent>();
		m_EntityWorld.RegisterComponent<component::CameraComponent>();

		//m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::LevelSystem>(*m_pResourceManager);
		//m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_GLFWWindow);
		m_EntityWorld.RegisterSystem<ecs::TransformSystem>();
		m_EntityWorld.RegisterSystem<ecs::CameraSystem>();
	}

	void Application::Destroy()
	{
		//m_SFMLWindow->Destroy();
		m_EntityWorld.Destroy();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		m_EntityWorld.Update(gameTime);
		//m_ImGuiLog->Update();
	}

	void Application::Run(int argc, char* argv[])
	{
		Register();
		Init();
		PostInit();

		Saz::GameTime gameTime;

		while (true)
		{
			//gameTime.m_DeltaTime = glfwGetTime();
			gameTime.m_TotalTime += gameTime.m_DeltaTime;
			gameTime.m_Frame++;

			m_Window->BeginDrawing();

			Update(gameTime);

			m_Window->Update(gameTime);
			if (m_Window->ShouldClose())
				break;

			m_Window->EndDrawing();
		}
	}
}