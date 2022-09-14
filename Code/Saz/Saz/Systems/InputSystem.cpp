#include "SazPCH.h"
#include "InputSystem.h"

#include "Saz/InputComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/Core/WindowBase.h"

#include <entt/entt.hpp>
#include "TransformComponent.h"
#include "Saz/Core/GameTime.h"
#include "Saz/NameComponent.h"
#include "imgui.h"

namespace ecs
{	
	InputSystem::InputSystem(Saz::WindowBase& window)
		: m_Window(window)
	{

	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::Init()
	{
		ecs::Entity entity = m_World->CreateEntity();
		m_World->AddComponent<component::InputComponent>(entity);
	}

	void InputSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		m_KeyboardPrevious = std::move(m_KeyboardCurrent);
		m_MousePrevious = std::move(m_MouseCurrent);

		vec2 mouseDelta, mousePos;
		m_Window.GatherKeyboard(m_KeyboardCurrent);
		m_Window.GatherMouse(m_MouseCurrent, mouseDelta, mousePos);

		const auto view = registry.view<component::InputComponent>();
		for (const ecs::Entity& entity : view)
		{
			component::InputComponent& inputComponent = view.get<component::InputComponent>(entity);
			inputComponent.m_KeyboardCurrent = m_KeyboardCurrent;
			inputComponent.m_KeyboardPrevious = m_KeyboardPrevious;
			inputComponent.m_MousePrevious = m_MousePrevious;
			inputComponent.m_MouseCurrent = m_MouseCurrent;
			inputComponent.m_MouseDelta = mouseDelta;
			inputComponent.m_MousePosition = mousePos;
		}
	}
}
