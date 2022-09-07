#pragma once

#include "Saz/Core/KeyCodes.h"
#include "Saz/Core/MouseCodes.h"
#include <Core/Vector.h>

namespace Saz {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
