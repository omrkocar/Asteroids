#include "GamePCH.h"
#include "Player.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Core/GameTime.h"
#include "Level.h"
#include "glm/gtc/type_ptr.inl"

namespace Game
{
	
	Player::Player(Level* level)
	{
		m_Level = level;

		// Smoke
		m_SmokeParticle.Position = { 0.0f, 0.0f };
		m_SmokeParticle.Velocity = { -2.0f, 0.0f };
		m_SmokeParticle.VelocityVariation = { 4.0f, 2.0f };
		m_SmokeParticle.SizeBegin = 0.1f;
		m_SmokeParticle.SizeEnd = 0.0f;
		m_SmokeParticle.SizeVariation = 0.15f;
		m_SmokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };
		m_SmokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
		m_SmokeParticle.LifeTime = 4.0f;

		// Flames
		m_EngineParticle.Position = { 0.0f, 0.0f };
		m_EngineParticle.Velocity = { -10.0f, 0.0f }, m_EngineParticle.VelocityVariation = { -2.0f, 1.0f };
		m_EngineParticle.SizeBegin = 0.1f, m_EngineParticle.SizeEnd = 0.0f, m_EngineParticle.SizeVariation = 0.3f;
		m_EngineParticle.ColorBegin = { 0.975f, 0.066f, 0.038f, 1.0f };
		m_EngineParticle.ColorEnd = { 1.0f, 0.882f, 0.0f, 1.0f };
		m_EngineParticle.LifeTime = 0.15f;

		m_Speed = 4.0f;
	}

	Player::~Player()
	{
		
	}

	void Player::Init()
	{
		m_Position = { -0.7f, 0.0f };
		m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Ship.png");
	}

	void Player::OnUpdate(const Saz::GameTime& gameTime)
	{
		GameObject::OnUpdate(gameTime);

		m_Velocity = glm::vec2(0.0f);

		if (Saz::Input::IsKeyPressed(Saz::Key::W))
		{
			m_Velocity.y += m_Speed;
		}
		if (Saz::Input::IsKeyPressed(Saz::Key::S))
		{
			m_Velocity.y -= m_Speed;
		}
		if (Saz::Input::IsKeyPressed(Saz::Key::A))
		{
			m_Velocity.x -= m_Speed;
		}
		if (Saz::Input::IsKeyPressed(Saz::Key::D))
		{
			m_Velocity.x += m_Speed;
		}

		m_Position += m_Velocity * gameTime.GetDeltaTime();

		if (m_Position.y > 5.0f)
		{
			m_Position.y = 5.0f;
		}
		else if (m_Position.y < -5.0f)
		{
			m_Position.y = -5.0f;
		}

		m_EngineParticle.Velocity = { -10.0f, 0.0f };
		if (m_Velocity.x < 0)
		{
			m_EngineParticle.Velocity.x = -25.0f;
		}
		

		if (m_Velocity != glm::vec2(0.0f))
		{
			m_EngineParticle.Position = { m_Position.x - 0.5f, m_Position.y };
			m_ParticleSystem.Emit(m_EngineParticle);
		}

		m_ParticleSystem.OnUpdate(gameTime);
	}

	void Player::OnRender(Saz::OrthographicCamera& camera)
	{
		m_ParticleSystem.OnRender(camera);
		Saz::Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.0f }, m_Size, m_Texture);
	}

	void Player::OnImGuiRender()
	{
		ImGui::Begin("Profiler");
		auto stats = Saz::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats: ");
		ImGui::Text("DrawCalls: %d ", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_EngineParticle.ColorBegin));
		ImGui::ColorEdit4("Death Color", glm::value_ptr(m_EngineParticle.ColorEnd));
		ImGui::DragFloat("Life Time", &m_EngineParticle.LifeTime, 0.1f, 0.0f, 1000.0f);

		ImGui::End();
	}

	void Player::Reset()
	{
		m_ParticleSystem.Stop();
	}

}