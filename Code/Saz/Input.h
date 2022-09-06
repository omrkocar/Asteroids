#pragma once

#include <Saz/Core.h>

namespace Saz
{
	class SAZ_API Input
	{
	public:
		static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		static bool IsKeyReleased(int keycode) { return s_Instance->IsKeyReleasedImpl(keycode); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsKeyReleasedImpl(int keycode) = 0;

	private:
		static Input* s_Instance;
	};
}