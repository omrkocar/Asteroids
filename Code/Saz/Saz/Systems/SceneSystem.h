#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class SceneSystem final : public System
	{
	public:
		
		SceneSystem();

		virtual void Init();
		virtual void PostInit();
		virtual void Destroy();

		virtual void Update(const Saz::GameTime& gameTime);
	};
}
