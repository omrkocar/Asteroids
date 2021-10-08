#include "SazPCH.h"
#include "TransformSystem.h"

#include "Saz/TransformComponent.h"
#include "Saz/InputComponent.h"

#include <entt/entt.hpp>

namespace ecs
{	
	TransformSystem::TransformSystem()
	{

	}

	void TransformSystem::Init()
	{

	}

	void TransformSystem::Update()
	{
		auto& registry = m_World->m_Registry;
		const auto view = registry.view<component::TransformComponent, component::InputComponent>();
		for (const ecs::Entity& entity : view)
		{
			auto& transformComponent = view.get<component::TransformComponent>(entity);
			auto& inputComponent = view.get<component::InputComponent>(entity);
			vec2& pos = transformComponent.m_Position;

			if (inputComponent.IsKeyHeld(Input::EKeyboard::A))
				pos.x -= 0.5f;
			if (inputComponent.IsKeyHeld(Input::EKeyboard::D))
				pos.x += 0.5f;
			if (inputComponent.IsKeyHeld(Input::EKeyboard::W))
				pos.y += 0.5f;
			if (inputComponent.IsKeyHeld(Input::EKeyboard::S))
				pos.y -= 0.5f;
		}
	}

}
