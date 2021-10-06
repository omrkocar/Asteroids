#pragma once

namespace sf
{
	class Texture;
}

namespace Saz
{
	class ResourceManager final
	{
	public:
		ResourceManager();
		~ResourceManager();

		sf::Texture* GetTexture(const String& name);
		sf::Texture* LoadTexture(const String& filename);

	private:
		Map<String, sf::Texture*> m_Textures;
	};
}
