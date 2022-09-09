#pragma once

#include <Saz/Core/WindowBase.h>

struct GLFWwindow;

namespace Saz
{
	class GraphicsContext;

	class SAZ_API WindowsWindow final : public WindowBase
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