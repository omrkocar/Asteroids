#include "SazPCH.h"
#include "InputSystem.h"

#include "Saz/InputComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/Window.h"


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

		
	}
}
