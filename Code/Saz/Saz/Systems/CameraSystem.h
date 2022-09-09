#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class CameraSystem final : public System
	{
	public:
		
		CameraSystem();
		virtual void Destroy() override;

		virtual void Update(const Saz::GameTime& gameTime) override;
	};
}
