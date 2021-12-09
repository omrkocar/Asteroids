#include "SazPCH.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"

#include "Saz/Window.h"

#include <entt/entt.hpp>

namespace ecs
{
	RenderSystem::RenderSystem(Saz::Window& window)
		: m_Window(window)
	{
		
	}

	RenderSystem::~RenderSystem()
	{
		
	}

	void RenderSystem::PostInit()
	{
		
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;
		const auto view = registry.view<component::RenderComponent, component::TransformComponent>();
		for (const ecs::Entity& entity : view)
		{
			component::RenderComponent& renderComp = view.get<component::RenderComponent>(entity);
			component::TransformComponent& transformComp = view.get<component::TransformComponent>(entity);
			
			renderComp.texture->Draw(::Vector2{ transformComp.m_Position.x, transformComp.m_Position.y });
		}
	}
}
