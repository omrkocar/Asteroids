#include "SazPCH.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"

#include "Saz/WindowBase.h"

#include <entt/entt.hpp>
#include "InputComponent.h"

namespace ecs
{
	RenderSystem::RenderSystem(Saz::WindowBase& window)
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

		const auto cameraView = registry.view<component::CameraComponent>();
		for (const ecs::Entity& cameraEntity : cameraView)
		{
			component::CameraComponent& cameraComponent = cameraView.get<component::CameraComponent>(cameraEntity);
			//cameraComponent.camera2D->Update();
			const auto view = registry.view<component::RenderComponent, component::TransformComponent>();
			for (const ecs::Entity& entity : view)
			{
				component::RenderComponent& renderComp = view.get<component::RenderComponent>(entity);
				component::TransformComponent& transformComp = view.get<component::TransformComponent>(entity);


				const auto inputView = registry.view<component::InputComponent>();
				for (const ecs::Entity& inputEntity : inputView)
				{
				}
			}

		}

		
	}
}
