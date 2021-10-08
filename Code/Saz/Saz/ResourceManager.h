#pragma once

#include <SFML/Graphics/Color.hpp>

namespace sf
{
	class Texture;
	class RectangleShape;
	class CircleShape;
	class Color;
	class Sprite;
	class Shape;
}

namespace Saz
{
	class ResourceManager final
	{
	public:
		ResourceManager();
		~ResourceManager();

		const sf::Texture& GetTexture(const String& name);
		const sf::Texture& LoadTexture(const String& filename);

		sf::Sprite* CreateSprite();

		const sf::Shape* GetShape(const String& name);
		sf::RectangleShape* CreateRectangle(const String& name, vec2 size, sf::Color color = sf::Color::White);
		sf::CircleShape* CreateCircle(const String& name, float radius, std::size_t pointCount);

	private:
		Map<String, sf::Texture> m_Textures;
		DynamicArray<sf::Sprite*> m_Sprites;
		Map<String,sf::Shape*> m_Shapes;
	};
}
