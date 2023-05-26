#pragma once

#include <Saz/Core/KeyCodes.h>
#include <Saz/Core/MouseCodes.h>
#include <Core/Set.h>
#include <Core/Vector.h>

namespace component
{
	struct InputComponent
	{
		bool IsKeyHeld(Input::KeyCode key) const;
		bool IsKeyPressed(Input::KeyCode key) const;
		bool IsKeyReleased(Input::KeyCode key) const;

		bool IsKeyHeld(Input::MouseCode key) const;
		bool IsKeyPressed(Input::MouseCode key) const;
		bool IsKeyReleased(Input::MouseCode key) const;

		const Vector2& GetMousePosition() { return m_MousePosition; }
		const Vector2& GetMouseDelta() { return m_MouseDelta; }

		Set<Input::KeyCode> m_KeyboardPrevious;
		Set<Input::KeyCode> m_KeyboardCurrent;
		Set<Input::MouseCode> m_MousePrevious;
		Set<Input::MouseCode> m_MouseCurrent;

		Vector2 m_MouseDelta;
		Vector2 m_MousePosition;
	};
}
