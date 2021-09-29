#include "GamePCH.h"
#include "SpriteComponent.h"

namespace
{
	static constexpr const char* initialPath = "../Textures/";
}


SpriteComponent::SpriteComponent(const std::string& textureName)
{
	m_pTexture = new sf::Texture();
	if (!m_pTexture->loadFromFile(initialPath + textureName))
	{
		// Error
	}

	m_pSprite = new sf::Sprite();
	m_pSprite->setTexture(*m_pTexture);
}

void SpriteComponent::Init()
{

}

