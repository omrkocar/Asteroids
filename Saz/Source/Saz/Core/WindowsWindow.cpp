#include "SazPCH.h"
#include "WindowsWindow.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Saz
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
		: WindowBase(props)
	{
		glfwInit();

		m_Size = props.Size;
		m_Name = props.Title;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(props.Size.x, props.Size.y, m_Name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, FrameBufferResizeCallback);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate(const Saz::GameTime& gameTime)
	{
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		
	}

	bool WindowsWindow::IsVSync() const
	{
		return true;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void WindowsWindow::GatherKeyboard(Set<Input::KeyCode>& out_Keys) const
	{

	}

	void WindowsWindow::GatherMouse(Set<Input::MouseCode>& out_Keys, Vector2& out_Delta, Vector2& out_Position) const
	{

	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}


	bool WindowsWindow::HasResized() const
	{
		return m_Resized;
	}

	Vector2Int WindowsWindow::GetSize() const
	{
		return m_Size;
	}

	void WindowsWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		glfwCreateWindowSurface(instance, m_Window, nullptr, surface);
	}

	void WindowsWindow::FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		auto* window = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(glfwWindow));
		window->m_Size.x = static_cast<uint32_t>(width);
		window->m_Size.y = static_cast<uint32_t>(height);
		window->m_Resized = true;
	}
}