#pragma once

#include <Saz/System.h>

namespace Saz
{
	class ResourceManager;
	class GameTime;
}

namespace ecs 
{
	class SAZ_API LevelSystem final : public System
	{
	public:
		
		LevelSystem(Saz::ResourceManager& resourceManager);

		bool LoadFromFile(const String& filename);

	private:
		Saz::ResourceManager& m_ResourceManager;
	};
}
