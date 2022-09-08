#include "GamePCH.h"
#include "Player.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Core/GameTime.h"

namespace Game
{
	
	Player::Player()
	{

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

		if (Saz::Input::IsKeyPressed(Saz::Key::W))
		{
			m_Position.y += m_Speed * gameTime.GetDeltaTime();
		}
		if (Saz::Input::IsKeyPressed(Saz::Key::S))
		{
			m_Position.y -= m_Speed * gameTime.GetDeltaTime();
		}

		if (m_Position.y > 5.0f)
		{
			m_Position.y = 5.0f;
		}
		else if (m_Position.y < -5.0f)
		{
			m_Position.y = -5.0f;
		}
	}

	void Player::OnRender()
	{
		Saz::Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.0f }, m_Size, m_Texture);
	}

	void Player::OnImGuiRender()
	{
		ImGui::DragFloat("MoveSpeed", &m_Speed, 0.1f);
	}

	void Player::Reset()
	{

	}

}