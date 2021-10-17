#pragma once

#include <Saz/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class WorldOutliner final : public System
	{
	public:
		
		WorldOutliner();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
	};
}
