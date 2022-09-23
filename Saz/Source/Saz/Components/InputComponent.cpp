#include "SazPCH.h"
#include "InputComponent.h"

bool component::InputComponent::IsKeyHeld(Input::KeyCode key) const
{
	return m_KeyboardCurrent.count(key) > 0;
}

bool component::InputComponent::IsKeyPressed(Input::KeyCode key) const
{
	return m_KeyboardPrevious.count(key) == 0
		&& m_KeyboardCurrent.count(key) > 0;
}

bool component::InputComponent::IsKeyReleased(Input::KeyCode key) const
{
	return m_KeyboardPrevious.count(key) > 0
		&& m_KeyboardCurrent.count(key) == 0;
}

bool component::InputComponent::IsKeyHeld(Input::MouseCode key) const
{
	return m_MouseCurrent.count(key) > 0;
}

bool component::InputComponent::IsKeyPressed(Input::MouseCode key) const
{
	return m_MousePrevious.count(key) == 0
		&& m_MouseCurrent.count(key) > 0;
}

bool component::InputComponent::IsKeyReleased(Input::MouseCode key) const
{
	return m_MousePrevious.count(key) > 0
		&& m_MouseCurrent.count(key) == 0;
}