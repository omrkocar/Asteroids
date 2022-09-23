#pragma once

#include <Core/String.h>
#include "Core/Set.h"
#include "Core/Vector.h"

namespace Input
{
	enum class KeyCode : uint16_t;
	enum class MouseCode : uint16_t;
}
namespace Saz
{
	class GameTime;

	struct WindowSpecification
	{
		String Title = "Saz Engine";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Decorated = true;
		bool Fullscreen = false;
		bool VSync = true;
	};

	// Interface representing a desktop system based Window
	class WindowBase
	{
	public:
		WindowBase() = delete;
		WindowBase(const WindowSpecification& specification) {}
		virtual ~WindowBase() {}

		virtual void Init() {}
		virtual void PostInit() {}
		virtual void Destroy() {}

		virtual void OnUpdate(const Saz::GameTime& gameTime) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;
		
		virtual void SetResizable(bool resizable) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual bool ShouldClose() const = 0;

		virtual void Shutdown() = 0;

		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;

		virtual void GatherKeyboard(Set<Input::KeyCode>& out_Keys) const = 0;
		virtual void GatherMouse(Set<Input::MouseCode>& out_Keys, vec2& out_Delta, vec2& out_Position) const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<WindowBase> Create(const WindowSpecification& props = WindowSpecification());

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;
	};
}