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
		StringView Title;
		Vector2Int Size;

		WindowProps(const String& title = "Saz Engine", Vector2Int size = Vector2Int(1920, 1080))
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
		virtual void Shutdown() {};

		virtual void OnUpdate(const Saz::GameTime& gameTime) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual Vector2Int GetSize() const = 0;

		virtual bool ShouldClose() const = 0;
		virtual bool HasResized() const = 0;

		virtual void GatherKeyboard(Set<Input::KeyCode>& out_Keys) const = 0;
		virtual void GatherMouse(Set<Input::MouseCode>& out_Keys, Vector2& out_Delta, Vector2& out_Position) const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Saz::WindowBase* Create(const WindowProps& props = WindowProps());

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;
	};
}