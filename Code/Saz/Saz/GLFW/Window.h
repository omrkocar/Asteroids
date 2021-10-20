#pragma once

#include <Saz/WindowBase.h>

#include <Vulkan/vulkan.h>

namespace Input
{
	enum class EKeyboard;
	enum class EMouse;
}

namespace Saz
{
	class MyGLContext;
}

struct GLFWwindow;

namespace Saz::glfw
{
	class Window final : public Saz::WindowBase
	{

	public:
		explicit Window(const Saz::WindowProps& props);
		~Window() override;

		void Update(const Saz::GameTime& gameTime) override;

		ivec2 GetSize() const override { return m_Size; }
		void SetSize(ivec2 newSize) override { m_Size = newSize; }

		bool ShouldClose() const override;

		void GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const;
		void GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const;

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		bool HasResized() const override { return m_HasResized; }
		void SetResized(bool value) { m_HasResized = value; }

	private:
		static void FramebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

	private:
		String m_Title = { };
		ivec2 m_Size = { };

		vec2 m_MousePos = vec2(0.0f);
		vec2 m_MouseDelta = vec2(0.0f);

		Saz::MyGLContext* m_pMyGLContext;

		bool m_HasResized = false;

		GLFWwindow* m_Window = nullptr;
	};
}