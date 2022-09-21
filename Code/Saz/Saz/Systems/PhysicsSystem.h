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
		~PhysicsSystem();
		virtual void Update(const Saz::GameTime& gameTime) override;

		void OnRuntimeStart_UNIT_TEST();
		void OnRuntimeStop_UNIT_TEST();

		b2World* GetPhysicsWorld() const { return m_PhysicsWorld; }
		bool IsActive() { return m_IsActive; }
	private:
		void OnSceneStateChanged(Entity entity);
		void OnRuntimeStart();
		void OnRuntimeStop();
	private:
		Entity m_Entity;
		b2World* m_PhysicsWorld = nullptr;
		bool m_IsActive = false;
	};
}
