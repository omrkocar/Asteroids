#include "SazPCH.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"

#include "SFML/Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <entt/entt.hpp>

namespace ecs
{	
	RenderSystem::RenderSystem(Saz::sfml::Window& sfmlWindow)
		: m_SFMLWindow(sfmlWindow)
	{
	
	}

	RenderSystem::~RenderSystem()
	{
	}

	void RenderSystem::Init()
	{
		
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		RenderSFML();
	}

	void RenderSystem::RenderSFML()
	{
		//auto& registry = m_World->m_Registry;

		//const auto cameraView = registry.view<component::CameraComponent, component::TransformComponent>();
		//for (const ecs::Entity& cameraEntity : cameraView)
		//{
		//	// Render Comp
		//	{
		//		const auto renderView = registry.view<component::RenderComponent, component::TransformComponent>();
		//		for (const ecs::Entity& view : renderView)
		//		{
		//			const auto& renderComp = renderView.get<component::RenderComponent>(view);
		//			const auto& transformComponent = renderView.get<component::TransformComponent>(view);

		//			sf::Sprite* sprite = renderComp.m_Sprite;
		//			const sf::Texture* texture = renderComp.m_Sprite->getTexture();
		//			if (sprite && texture)
		//			{
		//				sprite->setPosition(transformComponent.m_Position.x, transformComponent.m_Position.y);
		//				sprite->setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
		//				sprite->setScale(transformComponent.m_Scale.x, transformComponent.m_Scale.y * -1.0f);
		//				m_SFMLWindow.m_Texture.draw(*sprite);
		//			}
		//		}
		//	}
		//}
	}
}
