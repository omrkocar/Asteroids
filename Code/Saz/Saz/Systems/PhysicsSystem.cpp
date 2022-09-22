#include "SazPCH.h"
#include "PhysicsSystem.h"

#include "Saz/Components/PhysicsComponents.h"
#include "Saz/Components/TransformComponent.h"
#include "Saz/Components/SceneComponent.h"
#include "Saz/ProjectSettings/PhysicsSettings.h"

#include <box2d/box2d.h>
#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace
{
	constexpr int32_t velocityIterations = 6;
	constexpr int32_t positionIterations = 2;

	b2BodyType ConvertSazPhysicsTypeToBox2D(component::Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case component::Rigidbody2DComponent::BodyType::Static:
			return b2_staticBody;
		case component::Rigidbody2DComponent::BodyType::Dynamic:
			return b2_dynamicBody;
		case component::Rigidbody2DComponent::BodyType::Kinematic:
			return b2_kinematicBody;
		}

		SAZ_CORE_ASSERT(false, "Unknown physics type!");
		return b2_staticBody;
	}
}

namespace ecs
{

	PhysicsSystem::~PhysicsSystem()
	{
		if (m_PhysicsWorld != nullptr)
		{
			delete m_PhysicsWorld;
			m_PhysicsWorld = nullptr;
		}
	}

	void PhysicsSystem::Update(const Saz::GameTime& gameTime)
	{
		auto sceneStateView = m_World->GetAllEntitiesWith<component::SceneStateChangeRequestOneFrameComponent>();
		for (auto& entity : sceneStateView)
		{
			OnSceneStateChanged(entity);
		}

		if (!m_IsActive)
			return;

		m_PhysicsWorld->Step(gameTime.GetDeltaTime(), velocityIterations, positionIterations);

		auto view = m_World->GetAllEntitiesWith<component::Rigidbody2DComponent, component::TransformComponent>();
		for (auto& entity : view)
		{
			auto& transform = m_World->GetComponent<component::TransformComponent>(entity);
			auto& rigidbody2D = m_World->GetComponent<component::Rigidbody2DComponent>(entity);

			b2Body* body = (b2Body*)rigidbody2D.RuntimeBody;
			const auto& position = body->GetPosition();
			transform.Position.x = position.x;
			transform.Position.y = position.y;
			transform.Rotation.z = body->GetAngle();
		}
	}

	void PhysicsSystem::OnRuntimeStart_UNIT_TEST()
	{
		OnRuntimeStart();
	}

	void PhysicsSystem::OnRuntimeStop_UNIT_TEST()
	{
		OnRuntimeStop();
	}

	void PhysicsSystem::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ PhysicsSettings::Gravity[0], PhysicsSettings::Gravity[1] });
		auto view = m_World->GetAllEntitiesWith<component::Rigidbody2DComponent>();
		for (auto entity : view)
		{
			auto& transform = m_World->GetComponent<component::TransformComponent>(entity);
			auto& rigidbody2D = m_World->GetComponent<component::Rigidbody2DComponent>(entity);

			b2BodyDef bodyDef;
			bodyDef.type = ConvertSazPhysicsTypeToBox2D(rigidbody2D.Type);
			bodyDef.position.Set(transform.Position.x, transform.Position.y);
			bodyDef.angle = transform.Rotation.z;
			bodyDef.gravityScale = rigidbody2D.GravityScale;
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rigidbody2D.FixedRotation);
			rigidbody2D.RuntimeBody = body;

			if (m_World->HasComponent<component::BoxCollider2DComponent>(entity))
			{
				auto& boxCollider = m_World->GetComponent<component::BoxCollider2DComponent>(entity);

				b2PolygonShape boxShape;
				boxShape.SetAsBox(transform.Scale.x * boxCollider.Size.x, transform.Scale.y * boxCollider.Size.y);
				bodyDef.angle = transform.Rotation.z;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider.Density;
				fixtureDef.friction = boxCollider.Friction;
				fixtureDef.restitution = boxCollider.Restitution;
				fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (m_World->HasComponent<component::CircleCollider2DComponent>(entity))
			{
				auto& circleCollider = m_World->GetComponent<component::CircleCollider2DComponent>(entity);

				b2CircleShape circleShape;
				circleShape.m_p.Set(circleCollider.Offset.x, circleCollider.Offset.y);
				circleShape.m_radius = transform.Scale.x * circleCollider.Radius;
				bodyDef.angle = transform.Rotation.z;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = circleCollider.Density;
				fixtureDef.friction = circleCollider.Friction;
				fixtureDef.restitution = circleCollider.Restitution;
				fixtureDef.restitutionThreshold = circleCollider.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}

		m_IsActive = true;
	}

	void PhysicsSystem::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
		m_IsActive = false;
	}

	void PhysicsSystem::OnSceneStateChanged(Entity entity)
	{
		auto& sceneStateChangedComp = m_World->GetComponent<component::SceneStateChangeRequestOneFrameComponent>(entity);

		if (sceneStateChangedComp.SceneState == SceneState::Play)
			OnRuntimeStart();
		else if (sceneStateChangedComp.SceneState == SceneState::Editor || sceneStateChangedComp.SceneState == SceneState::ForceStop)
			OnRuntimeStop();
	}
}
