#pragma once

#include <Saz/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class SAZ_API CameraSystem final : public System
	{
	public:
		
		CameraSystem();
		virtual void Destroy() override;

		virtual void Update(const Saz::GameTime& gameTime) override;
	};
}
