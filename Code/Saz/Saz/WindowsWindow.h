#pragma once

#include <Saz/WindowBase.h>

struct GLFWwindow;

namespace Input
{
	enum class EKeyboard;
	enum class EMouse;
}

namespace Saz
{
	class SAZ_API WindowsWindow final : public Saz::WindowBase
	{
	public:
		explicit WindowsWindow(const Saz::WindowProps& props);
		virtual ~WindowsWindow() override;

		void Update(const Saz::GameTime& gameTime);

		virtual void SetVSync(bool enabled);
		virtual bool IsVSync() const;
		virtual void SetResizable(bool resizable) const;

		virtual const std::string& GetTitle() const;
		virtual void SetTitle(const std::string& title);

		inline virtual unsigned int GetWidth() const;
		inline virtual unsigned int GetHeight() const;

		bool ShouldClose() const override;

		void GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const;
		void GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const;

		bool HasResized() const { return m_HasResized; }
		void SetResized(bool value) { m_HasResized = value; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		String m_Title = { };
		ivec2 m_Size = { };

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		vec2 m_MousePos = vec2(0.0f);
		vec2 m_MouseDelta = vec2(0.0f);

		bool m_HasResized = false;
	};
}