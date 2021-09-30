#pragma once

#include "Manager.h"

class Component;

namespace sf
{
	class Texture;
	class Sprite;
}

namespace sf
{
	class RenderWindow;
}

class ResourceManager : public Manager
{
public:
	ResourceManager();
	~ResourceManager();

	void LoadTexture(const std::string& fileName, sf::Texture* pTexture);

	sf::Texture* GetTexture(const std::string& name);

	void AddTexture(const std::string& name, sf::Texture* pTexture);

protected:
	std::map<std::string, sf::Texture*> m_pTextures;
	std::map<std::string, sf::Sprite*> m_pSprites;
	
};