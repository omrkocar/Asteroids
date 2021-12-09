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

		virtual void Update(const Saz::GameTime& gameTime) override;

		bool m_IsActive = true;
	};
}
