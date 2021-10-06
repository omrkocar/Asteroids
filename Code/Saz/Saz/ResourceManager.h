#pragma once

namespace sf
{
	class Texture;
}

namespace Saz
{
	// singleton
	class ResourceManager final
	{
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		sf::Texture* GetTexture(const String& name);
		sf::Texture* CreateTexture(const String& filename);

	private:
		Map<String, sf::Texture*> m_Textures;
		static ResourceManager* s_Instance;
	};
}
