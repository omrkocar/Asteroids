#pragma once

#include <Saz/Core/EntityWorld.h>

namespace ecs
{
	class System
	{
		friend class ecs::EntityWorld;

	public:
		virtual ~System() {}

		virtual void Init() {}
		virtual void Destroy() {}

		virtual void Update(const Saz::GameTime& gameTime) {}

	protected:
		ecs::EntityWorld* m_World = nullptr;
	};
}
