#pragma once


class Background : public sf::Drawable
{
public:
	Background();
	~Background();

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::VertexArray m_vertices;
};