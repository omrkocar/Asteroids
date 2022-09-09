#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class ResourceManager;
	class GameTime;
}

namespace ecs 
{
	class LevelSystem final : public System
	{
	public:
		
		LevelSystem(Saz::ResourceManager& resourceManager);

		bool LoadFromFile(const String& filename);

	private:
		Saz::ResourceManager& m_ResourceManager;
	};
}
