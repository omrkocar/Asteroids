#include "SazPCH.h"
#include "InputSystem.h"

#include "Saz/InputComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/Window.h"


#include <entt/entt.hpp>
#include "TransformComponent.h"
#include "GameTime.h"

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
		
	}

	void InputSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;
		// Temporary
		const auto view = registry.view<component::InputComponent, component::TransformComponent, component::MovementComponent>();
		for (const ecs::Entity& entity : view)
		{
			component::TransformComponent& transformComp = view.get<component::TransformComponent>(entity);
			component::MovementComponent& movementComp = view.get<component::MovementComponent>(entity);
			float moveSpeed = movementComp.m_Speed;
			if (IsKeyDown(KEY_D))
			{
				transformComp.m_Position.x += moveSpeed * gameTime.m_DeltaTime;
			}
			if (IsKeyDown(KEY_A))
			{
				transformComp.m_Position.x -= moveSpeed * gameTime.m_DeltaTime;
			}
			if (IsKeyDown(KEY_W))
			{
				transformComp.m_Position.y -= moveSpeed * gameTime.m_DeltaTime;
			}
			if (IsKeyDown(KEY_S))
			{
				transformComp.m_Position.y += moveSpeed * gameTime.m_DeltaTime;
			}
		}
	}
}
