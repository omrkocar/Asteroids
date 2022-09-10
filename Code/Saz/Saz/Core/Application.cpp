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
#include "Saz/Systems/SceneSystem.h"
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
#include "SceneComponent.h"

namespace Saz
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const String& name)
	{
		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Saz::Log::Init();

		m_Window = WindowBase::Create(WindowProps(name));
	}

	Application::~Application()
	{
		Destroy();
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

		if (!m_Minimized)
		{
			m_EntityWorld.Update(gameTime);
		}

		m_ImGuiLayer->Begin();
		m_EntityWorld.ImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate(gameTime);
	}

	/*void Application::OnEvent(Event& e)
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
	}*/

	void Application::Run()
	{
		Register();
		Init();
		PostInit();

		Saz::GameTime gameTime;

		while (!m_Window->ShouldClose())
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
		m_EntityWorld.RegisterComponent<component::SceneComponent>();
		m_EntityWorld.RegisterComponent<component::MovementComponent>();
		m_EntityWorld.RegisterComponent<component::NameComponent>();
		m_EntityWorld.RegisterComponent<component::SpriteRendererComponent>();
		m_EntityWorld.RegisterComponent<component::TransformComponent>();

		m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_Window);
		m_EntityWorld.RegisterSystem<ecs::CameraSystem>();
		m_EntityWorld.RegisterSystem<ecs::SceneSystem>();

		IMGUI_LOG_INFO("Registered Engine Systems and Components");
	}

	ecs::EntityWorld& Application::GetWorld()
	{
		return m_EntityWorld;
	}

	/*bool Application::OnWindowClose(WindowCloseEvent& e)
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
	}*/

	void Application::Close()
	{
		m_Running = false;
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