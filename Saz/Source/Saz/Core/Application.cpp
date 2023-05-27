#include "SazPCH.h"
#include "Application.h"

#include "Saz/Core/GameTime.h"
#include "Saz/Core/WindowsWindow.h"
#include "Saz/Vulkan/Device.h"
#include "Saz/Vulkan/Pipeline.h"
#include "Saz/Vulkan/SwapChain.h"
#include "Saz/Vulkan/Renderer.h"

#include <GLFW/glfw3.h>

namespace Saz
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const String& name)
	{
		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		Saz::Log::Init();

		WindowProps props;
		props.Title = name;
		props.Size = Vector2Int(1920, 1080);
		// TODO OK: Move these into Renderer
		m_Window = std::make_unique<WindowsWindow>(props);
		m_Device = std::make_unique<vulkan::Device>(*m_Window);
		m_Renderer = std::make_unique<vulkan::Renderer>(*m_Window, *m_Device);
	}

	Application::~Application()
	{
		Destroy();
	}

	void Application::Init()
	{
		//Renderer::Init();
	}

	void Application::PostInit()
	{

		m_Window->PostInit();
	}

	void Application::Register()
	{
	}

	void Application::Destroy()
	{
		m_Window->Destroy();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
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
}