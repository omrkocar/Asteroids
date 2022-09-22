#pragma once
#include <glm/ext/vector_float2.hpp>

namespace Physics
{
	enum class BodyType { Static = 0, Dynamic, Kinematic };
}

namespace component
{
	struct Rigidbody2DComponent
	{

		Physics::BodyType Type = Physics::BodyType::Static;
		bool FixedRotation = false;
		float GravityScale = 1.0f;

		// Storage for runtime
		void* RuntimeBody = nullptr;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Size   = { 0.5f, 0.5f };
		glm::vec2 Offset = { 0.0f, 0.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;
	};

	struct CircleCollider2DComponent
	{
		float Radius = 0.5f;
		glm::vec2 Offset = { 0.0f, 0.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;
	};
}