#pragma once

#include <Saz/System.h>

namespace Saz
{
	class GameTime;

	namespace glfw
	{
		class Window;
	}
}

namespace ecs 
{

	class SAZ_API InputSystem final : public System
	{
	public:
		
		InputSystem(Saz::glfw::Window& glfwWindow);
		~InputSystem();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		Saz::glfw::Window& m_GLFWWindow;

		Set<Input::EKeyboard> m_KeyboardPrevious;
		Set<Input::EKeyboard> m_KeyboardCurrent;
		Set<Input::EMouse> m_MousePrevious;
		Set<Input::EMouse> m_MouseCurrent;
	};
}
