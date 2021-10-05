#pragma once

#include <Saz/WindowBase.h>

struct GLFWwindow;

namespace Input
{
	enum class EKeyboard;
	enum class EMouse;
}

namespace Saz::glfw
{
	// GLFW Window
	class SAZ_API Window final : public Saz::WindowBase
	{
	public:
		Window(const Saz::WindowProps& props);
		virtual ~Window() override;

		void Update() override;

		ivec2 GetSize() const override { return m_Size; }

		inline virtual bool HasResized() const override { return m_HasResized; }
		inline virtual void SetResized(bool value) { m_HasResized = value; }

		bool ShouldClose() const override;

		void GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const;
		void GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const;

	private:
		static void OnFrameBufferResized(GLFWwindow* glfwWindow, int width, int height);

	private:
		GLFWwindow* m_Window;

		ivec2 m_Size = vec2(0.0f);

		vec2 m_MousePos = vec2(0.0f);
		vec2 m_MouseDelta = vec2(0.0f);

		bool m_HasResized = false;

		String m_Title = {};
	};
}