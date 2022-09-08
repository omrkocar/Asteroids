#pragma once

#include "Game/AABB.h"

namespace Saz 
{ 
	class Texture2D;
	class GameTime; 
}

namespace Game
{
	class GameObject
	{
	public:
		virtual void Init();
		virtual void OnUpdate(const Saz::GameTime& gameTime);
		virtual void OnRender();
		virtual void OnImGuiRender();

		const glm::vec2& GetPosition() const { return m_Position; }
		float GetMoveSpeed() const { return m_Speed; }
		const AABB& GetAABB() const { return Bound; }

		virtual void Reset() {}

	protected:
		glm::vec2 m_Position{ 0.0f, 0.0f };
		glm::vec2 m_Size{ 1.0f, 1.0f };
		glm::vec2 m_Velocity{ 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_Speed = 10.0f;
		float m_RotationSpeed = 90.0f;
		Saz::Ref<Saz::Texture2D> m_Texture;

		AABB Bound;
	};
}