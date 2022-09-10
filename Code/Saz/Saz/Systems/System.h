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
		virtual void PostInit() {}
		virtual void Destroy() {}

		virtual void Update(const Saz::GameTime& gameTime) {}
		virtual void LateUpdate(const Saz::GameTime& gameTime) {}
		virtual void ImGuiRender() {}

	protected:
		ecs::EntityWorld* m_World = nullptr;
	};
}
