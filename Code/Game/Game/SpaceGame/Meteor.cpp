#include "GamePCH.h"
#include "Meteor.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Core/GameTime.h"
#include "Core/Random.h"

namespace Game
{

	Meteor::Meteor(Level& level)
		: m_Level(level)
	{
		Reset();
	}

	Meteor::~Meteor()
	{

	}

	void Meteor::OnUpdate(const Saz::GameTime& gameTime)
	{
		GameObject::OnUpdate(gameTime);

		m_Position.x -= m_Speed * gameTime.GetDeltaTime();
		m_Rotation += m_RotationSpeed * gameTime.GetDeltaTime();

		if (m_Position.x < -12.0f)
		{
			Reset();
		}

		if (Bound.Intersects(m_Level.GetPlayer().GetAABB()))
		{
			m_Level.OnCollision(*this);
		}
	}

	void Meteor::OnRender(Saz::OrthographicCamera& camera)
	{
		Saz::Renderer2D::DrawRotatedQuad({ m_Position.x, m_Position.y, -0.5f }, m_Size, m_Rotation, { 0.35f, 0.3f, 0.2f, 1.0f });
	}

	void Meteor::OnImGuiRender()
	{
		
	}

	void Meteor::Reset()
	{
		m_Position = { Random::Range(20.0f, 50.0f), Random::Range(-10.0f, 10.0f) };
		m_Speed = Random::Range(4.0f, 8.0f);
		m_RotationSpeed = Random::Range(45.0f, 180.0f);
	}
}