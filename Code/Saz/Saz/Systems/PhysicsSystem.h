#pragma once

#include <Saz/Systems/System.h>

class b2World;

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class PhysicsSystem final : public System
	{
	public:
		virtual void Init();
		virtual void Update(const Saz::GameTime& gameTime) override;

		void OnRuntimeStart();
		void OnRuntimeStop();
	private:
		void OnSceneStateChanged(entt::registry& registry, entt::entity entity);
	private:
		Entity m_Entity;
		b2World* m_PhysicsWorld = nullptr;
		bool m_IsActive = false;
	};
}
