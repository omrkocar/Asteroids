#pragma once

#include <Saz/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class SAZ_API TransformSystem final : public System
	{
	public:
		
		TransformSystem();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

		float speed;
	};
}
