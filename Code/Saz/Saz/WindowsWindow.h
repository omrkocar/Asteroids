#pragma once

#include <Saz/WindowBase.h>

struct GLFWwindow;

namespace Saz
{
	class GraphicsContext;

	class SAZ_API WindowsWindow final : public Saz::WindowBase
	{
	public:
		explicit WindowsWindow(const Saz::WindowProps& props);
		virtual ~WindowsWindow() override;

		void OnUpdate(const Saz::GameTime& gameTime) override;

		virtual void SetVSync(bool enabled);
		virtual bool IsVSync() const;

		virtual const std::string& GetTitle() const;
		virtual void SetTitle(const std::string& title);

		inline virtual unsigned int GetWidth() const;
		inline virtual unsigned int GetHeight() const;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}