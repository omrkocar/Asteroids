#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <optional>

namespace component
{
	struct RenderComponent
	{
		std::optional<sf::RectangleShape*> m_RectangleShape = std::nullopt;
		std::optional<sf::CircleShape*> m_CircleShape = std::nullopt;
		std::optional<sf::Sprite*> m_Sprite = std::nullopt;
	};
}


