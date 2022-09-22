#include "SazPCH.h"
#include "Application.h"

#include "Saz/Rendering/Renderer.h"
#include "Saz/Scripting/ScriptEngine.h"

#include <GLFW/glfw3.h>

namespace Saz
{
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
		Renderer::Init();
		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PostInit()
	{
		SAZ_PROFILE_FUNCTION();

		m_Window->PostInit();
	}

	void Application::Register()
	{
		SAZ_PROFILE_FUNCTION();
	}

	void Application::Destroy()
	{
		SAZ_PROFILE_FUNCTION();

		m_Window->Destroy();
		ScriptEngine::Shutdown();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		SAZ_PROFILE_FUNCTION();

		m_Window->OnUpdate(gameTime);
	}

	void Application::Run()
	{
		Register();
		Init();
		PostInit();

		Saz::GameTime gameTime;

		while (!m_Window->ShouldClose() && m_Running)
		{
			gameTime.m_TotalTime = (float)glfwGetTime();
			gameTime.m_DeltaTime = gameTime.m_TotalTime - m_LastFrameTime;
			gameTime.m_Frame++;
			m_LastFrameTime = gameTime.m_TotalTime;

			Update(gameTime);
		}
	}

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