#include "SazPCH.h"
#include "WindowsWindow.h"

#include "Saz/Core/GameTime.h"
#include "Saz/Screen.h"

#include "Saz/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#include "Core/KeyCodes.h"
#include "Rendering/Renderer.h"
#include "Core/Application.h"
#include "InputComponent.h"

namespace Saz
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SAZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
		: WindowBase(props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		SAZ_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SAZ_CORE_INFO("Creating Window {0} ({1}, {2}", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			s_GLFWInitialized = true;
			SAZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		glfwSetFramebufferSizeCallback(m_Window, OnWindowResized);
	}

	void WindowsWindow::PostInit()
	{
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				auto& world = Saz::Application::Get().GetWorld();
				if (!world.HasComponent<component::MouseScrollOneFrameComponent>(world.GetMainCameraEntity()))
				{
					auto& scrollComponent = world.AddComponent<component::MouseScrollOneFrameComponent>(world.GetMainCameraEntity());
					scrollComponent.YOffset = (float)yOffset;
				}		
			});
	}

	void WindowsWindow::Shutdown()
	{
		SAZ_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate(const GameTime& gameTime)
	{
		glfwPollEvents();

		double posX, posY;
		glfwGetCursorPos(m_Window, &posX, &posY);

		vec2 mousePos;
		mousePos.x = static_cast<float>(posX);
		mousePos.y = static_cast<float>(posY);
		m_MouseDelta = m_MousePos - mousePos;
		m_MousePos = mousePos;

		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool isEnabled)
	{
		if (isEnabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = isEnabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	const std::string& WindowsWindow::GetTitle() const
	{
		return m_Data.Title;
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	uint32_t WindowsWindow::GetWidth() const
	{
		return m_Data.Width;
	}

	uint32_t WindowsWindow::GetHeight() const
	{
		return m_Data.Height;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void WindowsWindow::OnWindowResized(GLFWwindow* glfwWindow, int width, int height)
	{
		auto* windowsWindow = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer((GLFWwindow*)glfwWindow));
		windowsWindow->m_Data.Width = static_cast<uint32_t>(width);
		windowsWindow->m_Data.Height = static_cast<uint32_t>(height);
		Screen::width = static_cast<float>(windowsWindow->m_Data.Width);
		Screen::height = static_cast<float>(windowsWindow->m_Data.Height);
		Renderer::OnWindowResize(width, height);
	}

	void WindowsWindow::GatherKeyboard(Set<Input::KeyCode>& out_Keys) const
	{
		for (uint16_t i = static_cast<uint16_t>(Input::KeyCode::A); i < static_cast<uint16_t>(Input::KeyCode::Max); ++i)
		{
			if (glfwGetKey(m_Window, i) == GLFW_PRESS)
				out_Keys.insert((Input::KeyCode)i);
		}
	}

	void WindowsWindow::GatherMouse(Set<Input::MouseCode>& out_Keys, vec2& out_Delta, vec2& out_Position) const
	{
		for (int32_t i = static_cast<uint16_t>(Input::MouseCode::ButtonFirst); i <= static_cast<uint16_t>(Input::MouseCode::ButtonLast); ++i)
		{
			if (glfwGetMouseButton(m_Window, i) == GLFW_PRESS)
				out_Keys.insert((Input::MouseCode)i);
		}

		out_Delta = m_MouseDelta;
		out_Position = m_MousePos;
	}

}

