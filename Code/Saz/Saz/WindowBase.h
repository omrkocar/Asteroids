#pragma once

#include <Saz/Core.h>

namespace Saz
{
	class GameTime;

	struct WindowProps
	{
		String Title = "Saz Engine";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Fullscreen = false;
		bool Vsync = true;

		WindowProps(const String& title = "Saz Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
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

		virtual void Update(const Saz::GameTime& gameTime) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetResizable(bool resizable) const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual bool ShouldClose() const { return false; }

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static WindowBase* Create(const WindowProps& props = WindowProps());

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;
	};
}