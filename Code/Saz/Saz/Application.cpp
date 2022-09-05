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
#include "Saz/WindowsWindow.h"

namespace Saz
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	const ecs::EntityWorld& Application::GetWorld()
	{
		return m_EntityWorld;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::Application()
	{
		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Saz::Log::Init();

		{
			Saz::WindowProps windowProps;
			windowProps.Title = "Saz Engine";
			windowProps.Width = static_cast<int>(Screen::width);
			windowProps.Height = static_cast<int>(Screen::height);
			m_Window = std::unique_ptr<WindowBase>(WindowsWindow::Create());
			m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		}
		
		//m_ImGuiLog = new imgui::Log();

		// #todo Create all textures with a single call here.
		m_pResourceManager = new Saz::ResourceManager();	
	}

	Application::~Application()
	{
		//delete m_ImGuiLog;
		delete m_pResourceManager;
	}

	void Application::Init()
	{
		m_EntityWorld.Init();
	}

	void Application::PostInit()
	{
		m_Window->PostInit();
		m_EntityWorld.PostInit();
	}

	void Application::Register()
	{
		m_EntityWorld.RegisterComponent<component::CameraComponent>();
		m_EntityWorld.RegisterComponent<component::InputComponent>();
		m_EntityWorld.RegisterComponent<component::LevelComponent>();
		m_EntityWorld.RegisterComponent<component::MovementComponent>();
		m_EntityWorld.RegisterComponent<component::NameComponent>();
		m_EntityWorld.RegisterComponent<component::RenderComponent>();
		m_EntityWorld.RegisterComponent<component::TransformComponent>();

		m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::LevelSystem>(*m_pResourceManager);
		m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::CameraSystem>();
	}

	void Application::Destroy()
	{
		m_Window->Destroy();
		m_EntityWorld.Destroy();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		m_EntityWorld.Update(gameTime);
		glClearColor(1, 0, 1, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate(gameTime);
		}

		m_Window->Update(gameTime);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//SAZ_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		Register();
		Init();
		PostInit();
		

		Saz::GameTime gameTime;

		while (m_Running)
		{

			gameTime.m_DeltaTime = 1 / 60.f;
			gameTime.m_TotalTime += gameTime.m_DeltaTime;
			gameTime.m_Frame++;

			Update(gameTime);
		}
	}
}