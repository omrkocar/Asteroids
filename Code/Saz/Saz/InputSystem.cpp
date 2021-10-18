#include "SazPCH.h"
#include "InputSystem.h"

#include "Saz/InputComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/GLFW/Window.h"


#include <entt/entt.hpp>

namespace ecs
{	
	InputSystem::InputSystem(Saz::glfw::Window& glfwWindow)
		: m_GLFWWindow(glfwWindow)
	{

	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::Init()
	{
		/*auto& registry = m_World->m_Registry;

		auto entity = m_World->CreateEntity();
		m_World->AddComponent<component::NameComponent>(entity).m_Name = "Input";
		m_World->AddComponent<component::InputComponent>(entity);*/
	}

	void InputSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		m_KeyboardPrevious = std::move(m_KeyboardCurrent);
		m_MousePrevious = std::move(m_MouseCurrent);

		vec2 glfwMouseDelta, glfwMousePos;
		m_GLFWWindow.GatherKeyboard(m_KeyboardCurrent);
		m_GLFWWindow.GatherMouse(m_MouseCurrent, glfwMouseDelta, glfwMousePos);

		auto& view = registry.view<component::InputComponent>();
		for (auto& entity : view)
		{
			auto& component = view.get<component::InputComponent>(entity);
			component.m_KeyboardPrevious = m_KeyboardPrevious;
			component.m_KeyboardCurrent = m_KeyboardCurrent;
			component.m_MousePrevious = m_MousePrevious;
			component.m_MouseCurrent = m_MouseCurrent;
			component.m_MouseDelta = glfwMouseDelta;
			component.m_MousePosition = glfwMousePos;
		}
	}
}
