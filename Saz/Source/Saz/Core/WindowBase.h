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

	struct WindowProps
	{
		String Title;
		ivec2 Size;

		WindowProps(const String& title = "Saz Engine", ivec2 size = ivec2(1920, 1080))
			: Title(title), Size(size)
		{}
	};

	// Interface representing a desktop system based Window
	class WindowBase
	{
	public:
		WindowBase() = delete;
		WindowBase(const WindowProps& props) {}
		virtual ~WindowBase() {}

		virtual void Init() {}
		virtual void PostInit() {}
		virtual void Destroy() {}

		virtual void OnUpdate(const Saz::GameTime& gameTime) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual const String& GetTitle() const = 0;
		virtual void SetTitle(const String& title) = 0;
		

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual bool ShouldClose() const = 0;

		virtual void Shutdown() = 0;

		virtual void GatherKeyboard(Set<Input::KeyCode>& out_Keys) const = 0;
		virtual void GatherMouse(Set<Input::MouseCode>& out_Keys, vec2& out_Delta, vec2& out_Position) const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Saz::WindowBase* Create(const WindowProps& props = WindowProps());

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;
	};
}