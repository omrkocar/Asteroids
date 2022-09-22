#include <Catch2/catch.hpp>

#include "Saz/Core/EntityWorld.h"
#include "Saz/Core/GameTime.h"
#include "Saz/Systems/System.h"

namespace
{
	struct ComponentA
	{
		bool m_Value = false;
	};

	class SystemA : public ecs::System
	{
	public:
		void Init() override { m_IsInitialized = true; }
		void Update(const Saz::GameTime& gameTime) override { m_IsUpdated = true; }
		void Destroy() override { m_IsDestroyed = true; }

	public:
		bool m_IsInitialized = false;
		bool m_IsDestroyed = false;
		bool m_IsUpdated = false;
	};
}

TEST_CASE("Entity World")
{
	SECTION("Initialize")
	{
		ecs::EntityWorld world;
		
		world.RegisterSystem<SystemA>();
		const auto& system = world.GetSystem<SystemA>();

		CHECK(!system.m_IsInitialized);

		world.Init();

		CHECK(system.m_IsInitialized);
	}

	SECTION("Update")
	{
		ecs::EntityWorld world;

		world.RegisterSystem<SystemA>();
		const auto& system = world.GetSystem<SystemA>();

		CHECK(!system.m_IsUpdated);

		world.Update({});

		CHECK(system.m_IsUpdated);
	}

	SECTION("Destroy")
	{
		ecs::EntityWorld world;

		world.RegisterSystem<SystemA>();
		const auto& system = world.GetSystem<SystemA>();

		CHECK(!system.m_IsDestroyed);

		world.Destroy();

		CHECK(system.m_IsDestroyed);
		
	}

	SECTION("CreateEntity")
	{
		ecs::EntityWorld world;
		CHECK(world.CreateEntity() == ecs::Entity(0));
		CHECK(world.CreateEntity() == ecs::Entity(1));
		CHECK(world.CreateEntity() == ecs::Entity(2));
	}
}
