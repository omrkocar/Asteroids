#include "SazPCH.h"
#include "WindowsWindow.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Saz
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
		: WindowBase(props)
	{
		Initialize(props);
	}

	void WindowsWindow::Initialize(const WindowProps& props)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(props.Size.x, props.Size.y, "Saz Engine", nullptr, nullptr);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::OnUpdate(const Saz::GameTime& gameTime)
	{
		glfwPollEvents();
	}

	void WindowsWindow::Destroy()
	{
		WindowBase::Destroy();

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{

	}

	bool WindowsWindow::IsVSync() const
	{
		return true;
	}

	const String& WindowsWindow::GetTitle() const
	{
		return nullptr;
	}

	void WindowsWindow::SetTitle(const String& title)
	{

	}

	uint32_t WindowsWindow::GetWidth() const
	{
		return 0;
	}

	uint32_t WindowsWindow::GetHeight() const
	{
		return 0;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void WindowsWindow::Shutdown()
	{

	}

	void WindowsWindow::GatherKeyboard(Set<Input::KeyCode>& out_Keys) const
	{

	}

	void WindowsWindow::GatherMouse(Set<Input::MouseCode>& out_Keys, Vector2& out_Delta, Vector2& out_Position) const
	{

	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return nullptr;
	}
}