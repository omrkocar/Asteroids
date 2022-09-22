#include <Catch2/catch.hpp>

#include <Saz/Core/EntityWorld.h>
#include <Saz/Systems/PhysicsSystem.h>
#include <Saz/Components/PhysicsComponents.h>
#include <Saz/Components/SceneComponent.h>
#include <Saz/Components/TransformComponent.h>
#include <Saz/Core/GameTime.h>
#include <Saz/ProjectSettings/PhysicsSettings.h>

#include <box2d/box2d.h>

TEST_CASE("Physics System")
{
	class ecs::EntityWorld world;

	world.RegisterComponent<component::Rigidbody2DComponent>();
	world.RegisterComponent<component::BoxCollider2DComponent>();
	world.RegisterComponent<component::CircleCollider2DComponent>();
	world.RegisterComponent<component::TransformComponent>();
	world.RegisterComponent<component::SceneStateChangeRequestOneFrameComponent>();

	world.RegisterSystem<ecs::PhysicsSystem>();
	ecs::PhysicsSystem& physicsSystem = world.GetSystem<ecs::PhysicsSystem>();
	world.Init();


	SECTION("Scene State: Play")
	{
		auto entity = world.CreateEntity();

		world.AddComponent<component::SceneStateChangeRequestOneFrameComponent>(entity, SceneState::Play);
		CHECK(physicsSystem.GetPhysicsWorld() == nullptr);
		CHECK(physicsSystem.IsActive() == false);
		world.Update({});
		CHECK(physicsSystem.GetPhysicsWorld() != nullptr);
		CHECK(physicsSystem.GetPhysicsWorld()->GetGravity().x == PhysicsSettings::Gravity[0]);
		CHECK(physicsSystem.GetPhysicsWorld()->GetGravity().y == PhysicsSettings::Gravity[1]);
		CHECK(physicsSystem.IsActive());

		world.DestroyEntity(entity);
	}

	SECTION("Scene State: Editor")
	{
		auto entity = world.CreateEntity();

		physicsSystem.OnRuntimeStart_UNIT_TEST();

		world.AddComponent<component::SceneStateChangeRequestOneFrameComponent>(entity, SceneState::Editor);
		CHECK(physicsSystem.GetPhysicsWorld() != nullptr);
		CHECK(physicsSystem.IsActive());
		world.Update({});
		CHECK(physicsSystem.GetPhysicsWorld() == nullptr);
		CHECK(physicsSystem.IsActive() == false);
	}

	SECTION("Scene State: ForceStop")
	{
		auto entity = world.CreateEntity();

		physicsSystem.OnRuntimeStart_UNIT_TEST();

		world.AddComponent<component::SceneStateChangeRequestOneFrameComponent>(entity, SceneState::ForceStop);
		CHECK(physicsSystem.GetPhysicsWorld() != nullptr);
		CHECK(physicsSystem.IsActive());
		world.Update({});
		CHECK(physicsSystem.GetPhysicsWorld() == nullptr);
		CHECK(physicsSystem.IsActive() == false);

		world.DestroyEntity(entity);
	}

	SECTION("Rigidbody setup")
	{
		auto entity = world.CreateEntity();

		auto& tc = world.AddComponent<component::TransformComponent>(entity);
		auto& rb = world.AddComponent<component::Rigidbody2DComponent>(entity);

		physicsSystem.OnRuntimeStart_UNIT_TEST();

		CHECK(rb.RuntimeBody != nullptr);

		world.DestroyEntity(entity);
	}
}