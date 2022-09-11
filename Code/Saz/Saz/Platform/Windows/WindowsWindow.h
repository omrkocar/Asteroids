#pragma once

#include <Saz/Core/WindowBase.h>
#include "Core/Vector.h"

struct GLFWwindow;

namespace Saz
{
	class GraphicsContext;

	class WindowsWindow final : public WindowBase
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		void OnUpdate(const GameTime& gameTime) override;

		virtual void SetVSync(bool enabled);
		virtual bool IsVSync() const;

		virtual const std::string& GetTitle() const override;
		virtual void SetTitle(const std::string& title) override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual bool ShouldClose() const override;
		void SetShouldClose(bool close) { m_ShouldClose = close; }

		static void OnWindowResized(GLFWwindow* glfwWindow, int width, int height);

		virtual void GatherKeyboard(Set<Input::KeyCode>& out_Keys) const;
		virtual void GatherMouse(Set<Input::MouseCode>& out_Keys, vec2& out_Delta, vec2& out_Position) const;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

		virtual void Shutdown() override;

	private:
		virtual void Init(const WindowProps& props);

	private:
		GLFWwindow* m_Window;
		Ref<GraphicsContext> m_Context;
		bool m_ShouldClose = false;

		vec2 m_MousePos = vec2::Zero();
		vec2 m_MouseDelta = vec2::Zero();

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;
		};

		WindowData m_Data;
	};
}