#pragma once

#include "Component.h"
#include <string>

namespace sf
{
	class Texture;
	class Sprite;
}

class SpriteComponent : public Component
{
public:
	SpriteComponent() = default;
	SpriteComponent(const std::string& textureName);

	static const char* GetStaticType() { return "SpriteComponent"; }
	virtual const char* GetType() override { return GetStaticType(); }
	static Component* Create() { return new SpriteComponent(); }

	virtual void Init() override;

	sf::Texture* GetTexture() { return m_pTexture; }
	sf::Sprite* GetSprite() { return m_pSprite; }

	void SetTexture(sf::Texture* pTexture) { m_pTexture = pTexture; }
	void SetSprite(sf::Sprite* pSprite) { m_pSprite = pSprite; }

protected:
	sf::Texture* m_pTexture;
	sf::Sprite* m_pSprite;
};