#include "GamePCH.h"
#include "SpriteComponent.h"
#include "Managers/ResourceManager.h"
#include "Actors/Actor.h"
#include "Core/World.h"
#include "Game.h"



SpriteComponent::SpriteComponent(const std::string& textureName)
{
	m_Name = textureName;
}

SpriteComponent::~SpriteComponent()
{
	delete m_pSprite;
}

void SpriteComponent::Init()
{
	ResourceManager* pResourceManager = m_pOwner->GetWorld()->GetGame()->GetResourceManager();

	m_pSprite = new sf::Sprite();
	m_pTexture = pResourceManager->GetTexture(m_Name);
	m_pSprite->setTexture(*m_pTexture);
	m_pSprite->setPosition(sf::Vector2f(400, 400));
}