#include "SazPCH.h"
#include "Application.h"

#include "Saz/CameraComponent.h"
#include "Saz/Core/EntityWorld.h"
#include "Saz/Core/GameTime.h"
#include "Saz/Systems/CameraSystem.h"
#include "Saz/InputComponent.h"
#include "Saz/Systems/InputSystem.h"
#include "Saz/Systems/RenderSystem.h"
#include "Saz/LevelComponent.h"
#include "Saz/Systems/LevelSystem.h"
#include "Saz/MovementComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/NameComponent.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"
#include "Saz/Platform/Windows/WindowsWindow.h"
#include "ImGui/ImGuiLog.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"
#include "Saz/Rendering/Renderer.h"

namespace Saz
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Saz::Log::Init();

		WindowProps windowProps;
		windowProps.Width = 2560;
		windowProps.Height = 1440;
		m_Window = std::unique_ptr<WindowBase>(WindowsWindow::Create(windowProps));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
		
	}

	void Application::Init()
	{
		m_EntityWorld.Init();

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PostInit()
	{
		SAZ_PROFILE_FUNCTION();

		m_Window->PostInit();
		m_EntityWorld.PostInit();
	}

	void Application::Destroy()
	{
		SAZ_PROFILE_FUNCTION();

		m_Window->Destroy();
		m_EntityWorld.Destroy();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		SAZ_PROFILE_FUNCTION();

		m_EntityWorld.Update(gameTime);

		if (!m_Minimized)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(gameTime);
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate(gameTime);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

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
			gameTime.m_TotalTime = (float)glfwGetTime();
			gameTime.m_DeltaTime = gameTime.m_TotalTime - m_LastFrameTime;
			gameTime.m_Frame++;
			m_LastFrameTime = gameTime.m_TotalTime;

			Update(gameTime);
		}
	}

	void Application::Register()
	{
		SAZ_PROFILE_FUNCTION();

		m_EntityWorld.RegisterComponent<component::CameraComponent>();
		m_EntityWorld.RegisterComponent<component::InputComponent>();
		m_EntityWorld.RegisterComponent<component::LevelComponent>();
		m_EntityWorld.RegisterComponent<component::MovementComponent>();
		m_EntityWorld.RegisterComponent<component::NameComponent>();
		m_EntityWorld.RegisterComponent<component::RenderComponent>();
		m_EntityWorld.RegisterComponent<component::TransformComponent>();

		m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::CameraSystem>();

		IMGUI_LOG_INFO("Registered Engine Systems and Components");
	}

	const ecs::EntityWorld& Application::GetWorld()
	{
		return m_EntityWorld;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;

			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
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
}