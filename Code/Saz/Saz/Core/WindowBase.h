#pragma once

#include <Core/String.h>
#include <Saz/Events/Event.h>

namespace Saz
{
	class GameTime;

	struct WindowProps
	{
		String Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const String& title = "Saz Engine",
					uint32_t width = 1280,
					uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	// Interface representing a desktop system based Window
	class SAZ_API WindowBase
	{
	public:
		WindowBase() = delete;
		WindowBase(const WindowProps& props) {}
		virtual ~WindowBase() {}
		
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void Init() {}
		virtual void PostInit() {}
		virtual void Destroy() {}

		virtual void OnUpdate(const Saz::GameTime& gameTime) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Scope<WindowBase> Create(const WindowProps& props = WindowProps());

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;
	};
}