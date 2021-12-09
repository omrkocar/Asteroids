#include "SazPCH.h"
#include "InputSystem.h"

#include "Saz/InputComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/Window.h"


#include <entt/entt.hpp>
#include "TransformComponent.h"

namespace ecs
{	
	InputSystem::InputSystem(Saz::Window& window)
		: m_Window(window)
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
		const auto view = registry.view<component::InputComponent, component::TransformComponent>();
		for (const ecs::Entity& entity : view)
		{
			component::TransformComponent& transformComp = view.get<component::TransformComponent>(entity);
			if (IsKeyDown(KEY_D))
			{
				transformComp.m_Position.x += 0.1f;
			}
			if (IsKeyDown(KEY_A))
			{
				transformComp.m_Position.x -= 0.1f;
			}
			if (IsKeyDown(KEY_W))
			{
				transformComp.m_Position.y -= 0.1f;
			}
			if (IsKeyDown(KEY_S))
			{
				transformComp.m_Position.y += 0.1f;
			}
		}
	}
}
