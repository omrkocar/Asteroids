#pragma once

#include <Saz/Core/EntityWorld.h>

namespace ecs
{
	class SAZ_API System
	{
		friend class ecs::EntityWorld;

	public:
		virtual ~System() {}

		virtual void Init() {}
		virtual void PostInit() {}
		virtual void Destroy() {}

		virtual void Update(const Saz::GameTime& gameTime) {};

	protected:
		ecs::EntityWorld* m_World = nullptr;
	};
}
