#include "GamePCH.h"
#include "Background.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Application.h"
#include "Saz/Core/GameTime.h"

namespace Game
{

	Background::Background()
	{
		
	}

	Background::~Background()
	{

	}

	void Background::Init()
	{
		Reset();
		m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Space.png");
	}

	void Background::OnUpdate(const Saz::GameTime& gameTime)
	{
		xPos1 -= m_ParallaxSpeed * gameTime.GetDeltaTime();
		xPos2 -= m_ParallaxSpeed * gameTime.GetDeltaTime();

		if (xPos1 < -m_Size)
		{
			xPos1 = m_Size;
		}
		else if (xPos2 < -m_Size)
		{
			xPos2 = m_Size;
		}
	}

	void Background::OnRender()
	{
		Saz::Renderer2D::DrawQuad({ xPos1, 0.0f, -0.8f }, { m_Size, 10.0f }, m_Texture);
		Saz::Renderer2D::DrawQuad({ xPos2, 0.0f, -0.8f }, { m_Size, 10.0f }, m_Texture);
	}

	void Background::OnImGuiRender()
	{
		ImGui::DragFloat("ParallaxSpeed", &m_ParallaxSpeed, 0.1f);
	}

	void Background::Reset()
	{
		xPos1 = 0.0f;
		xPos2 = 20.0f;
		m_ParallaxSpeed = 2.0f;
		m_Size = 20.0f;
	}

}