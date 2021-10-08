#pragma once

#include <Saz/System.h>

namespace ecs 
{
	class SAZ_API TransformSystem final : public System
	{
	public:
		
		TransformSystem();

		virtual void Init() override;
		virtual void Update() override;

	};
}
