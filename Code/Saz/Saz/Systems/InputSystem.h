#pragma once

#include <Saz/Systems/System.h>
#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>
#include <Core/Set.h>

namespace Saz
{
	class GameTime;

	class WindowBase;
	
}

namespace ecs 
{

	class InputSystem final : public System
	{
	public:
		
		InputSystem(Saz::WindowBase& window);
		~InputSystem();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		Saz::WindowBase& m_Window;

		Set<Input::KeyCode> m_KeyboardPrevious;
		Set<Input::KeyCode> m_KeyboardCurrent;
		Set<Input::MouseCode> m_MousePrevious;
		Set<Input::MouseCode> m_MouseCurrent;
	};
}