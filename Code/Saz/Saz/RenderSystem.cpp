#include "SazPCH.h"

#include "Saz/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/TransformComponent.h"

#include "SFML/Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <entt/entt.hpp>
#include "RenderComponents.h"

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
		auto& registry = m_World->m_Registry;

		const auto renderView = registry.view<component::RenderComponent, component::TransformComponent>();
		for (const ecs::Entity& view : renderView)
		{
			const auto& renderComp = renderView.get<component::RenderComponent>(view);
			const auto& transformComponent = renderView.get<component::TransformComponent>(view);
			
			if (renderComp.m_RectangleShape.has_value())
			{
				renderComp.m_RectangleShape.value()->setPosition(transformComponent.m_Position.x, transformComponent.m_Position.y);
				m_SFMLWindow.m_Texture.draw(*renderComp.m_RectangleShape.value());
			}
			else if (renderComp.m_CircleShape.has_value())
			{
				renderComp.m_CircleShape.value()->setPosition(transformComponent.m_Position.x, transformComponent.m_Position.y);
				m_SFMLWindow.m_Texture.draw(*renderComp.m_CircleShape.value());
			}
			else if (renderComp.m_Sprite.has_value())
			{
				sf::Sprite* sprite = renderComp.m_Sprite.value();
				sprite->setPosition(transformComponent.m_Position.x, transformComponent.m_Position.y);
				sprite->setOrigin(sprite->getTexture()->getSize().x / 2.0f, sprite->getTexture()->getSize().y / 2.0f);
				sprite->setScale(transformComponent.m_Scale.x, transformComponent.m_Scale.y * -1.0f);
				m_SFMLWindow.m_Texture.draw(*renderComp.m_Sprite.value());
			}
		}
	}
}
