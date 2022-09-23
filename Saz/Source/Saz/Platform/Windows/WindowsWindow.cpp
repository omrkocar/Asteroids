#include "SazPCH.h"
#include "WindowsWindow.h"

#include "Saz/Components/InputComponent.h"
#include "Saz/Core/Application.h"
#include "Saz/Core/KeyCodes.h"
#include "Saz/Platform/OpenGL/OpenGLContext.h"
#include "Saz/Rendering/Renderer.h"
#include "Saz/Screen.h"

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

namespace Saz
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SAZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowSpecification& specification)
		: WindowBase(specification), m_Specification(specification)
	{
		
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init()
	{
		SAZ_PROFILE_FUNCTION();

		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		SAZ_CORE_INFO("Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			SAZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		if (!m_Specification.Decorated)
		{
#ifdef SAZ_PLATFORM_WINDOWS
			glfwWindowHint(GLFW_TITLEBAR, false);
#else
			glfwWindowHint(GLFW_DECORATED, false);
#endif
		}

		if (m_Specification.Fullscreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

			glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			m_Window = glfwCreateWindow(mode->width, mode->height, m_Data.Title.c_str(), primaryMonitor, nullptr);
		}
		else
		{
			m_Window = glfwCreateWindow((int)m_Specification.Width, (int)m_Specification.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		glfwSetFramebufferSizeCallback(m_Window, OnWindowResized);

		// Update window size to actual size
		{
			int width, height;
			glfwGetWindowSize(m_Window, &width, &height);
			m_Data.Width = width;
			m_Data.Height = height;
		}

		// Set icon
		{
			GLFWimage icon;
			int channels;
			icon.pixels = stbi_load("Data/Textures/SazLogo.png", &icon.width, &icon.height, &channels, 4);
			glfwSetWindowIcon(m_Window, 1, &icon);
			stbi_image_free(icon.pixels);
		}
	}

	void WindowsWindow::PostInit()
	{
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.MouseWheel += (float)yOffset;
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

		m_Specification.VSync = isEnabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Specification.VSync;
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

	void WindowsWindow::SetResizable(bool resizable)
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
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

	void WindowsWindow::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void WindowsWindow::CenterWindow()
	{
		const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int x = (videmode->width / 2) - (m_Data.Width / 2);
		int y = (videmode->height / 2) - (m_Data.Height / 2);
		glfwSetWindowPos(m_Window, x, y);
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
		for (uint16_t i = static_cast<uint16_t>(Input::KeyCode::Space); i < static_cast<uint16_t>(Input::KeyCode::Max); ++i)
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

