#include "SazPCH.h"

#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>

#include <entt/entt.hpp>

namespace
{
	static constexpr const char* initialPath = "D:/Dev/Saz/Code/Game/Data/Textures/";
}

namespace Saz
{

	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		for (const auto& [name, texture] : m_Textures)
		{
			delete texture;
		}
	}

	Saz::ResourceManager* ResourceManager::GetInstance()
	{
		static ResourceManager* instance = new ResourceManager();
		return instance;
	}

	sf::Texture* ResourceManager::GetTexture(const String& name)
	{
		//SAZ_ASSERT(m_Textures, "No texture found with the given name!");
		return m_Textures[name];
	}


	sf::Texture* ResourceManager::CreateTexture(const String& filename)
	{
		sf::Texture* pTexture = new sf::Texture();
		pTexture->loadFromFile(initialPath + filename);
		m_Textures[filename] = pTexture;

		return pTexture;
	}

}