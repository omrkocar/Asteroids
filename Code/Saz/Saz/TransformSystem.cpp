#include "SazPCH.h"
#include "TransformSystem.h"

#include "Saz/GameTime.h"
#include "Saz/InputComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>

namespace ecs
{	
	TransformSystem::TransformSystem()
	{
		speed = 1.0f;
	}

	void TransformSystem::Init()
	{

	}

	void TransformSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;
		const auto view = registry.view<component::MovementComponent, component::TransformComponent, component::InputComponent>();
		for (const ecs::Entity& entity : view)
		{
			auto& transformComponent = view.get<component::TransformComponent>(entity);
			auto& inputComponent = view.get<component::InputComponent>(entity);
			auto& movementComponent = view.get<component::MovementComponent>(entity);

			ImGui::SliderFloat("MoveSpeed", &movementComponent.m_Speed, 100.0f, 2000.0f);
			vec2& pos = transformComponent.m_Position;

			// #todo: Move this logic into client
			if (inputComponent.IsKeyHeld(Input::EKeyboard::A))
				pos.x -= movementComponent.m_Speed * gameTime.m_DeltaTime;
			if (inputComponent.IsKeyHeld(Input::EKeyboard::D))
				pos.x += movementComponent.m_Speed * gameTime.m_DeltaTime;
			if (inputComponent.IsKeyHeld(Input::EKeyboard::W))
				pos.y += movementComponent.m_Speed * gameTime.m_DeltaTime;
			if (inputComponent.IsKeyHeld(Input::EKeyboard::S))
				pos.y -= movementComponent.m_Speed * gameTime.m_DeltaTime;
		}
	}
}
