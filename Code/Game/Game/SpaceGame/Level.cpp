#include "GamePCH.h"
#include "Level.h"
#include "Saz/Rendering/Renderer2D.h"
#include "glm/gtc/type_ptr.inl"
#include "Meteor.h"
#include "Player.h"

namespace Game
{
	
	Level::Level()
		: m_Player(this)
	{

	}

	Level::~Level()
	{
		for (auto meteor : m_meteors)
		{
			delete meteor;
		}
	}

	void Level::Init()
	{
		m_Background.Init();
		m_Player.Init();

		for (int i = 0; i < 30; i++)
		{
			m_meteors.push_back(new Meteor(*this));
		}
	}

	void Level::OnUpdate(const Saz::GameTime& gameTime)
	{
		if (m_GameState != EGameState::Running)
			return;

		m_Background.OnUpdate(gameTime);

		for (auto meteor : m_meteors)
		{
			meteor->OnUpdate(gameTime);
		}

		m_Player.OnUpdate(gameTime);
	}

	void Level::OnRender(Saz::OrthographicCamera& camera)
	{
		Saz::Renderer2D::BeginScene(camera);
		m_Background.OnRender();

		for (auto meteor : m_meteors)
		{
			meteor->OnRender(camera);
		}

		Saz::Renderer2D::EndScene();

		m_Player.OnRender(camera);
	}

	void Level::OnImGuiRender()
	{
		m_Background.OnImGuiRender();
		m_Player.OnImGuiRender();

		for (auto meteor : m_meteors)
		{
			meteor->OnImGuiRender();
		}
	}

	void Level::OnCollision(const Meteor& meteor)
	{
		m_GameState = EGameState::GameOver;
	}

	void Level::OnEvent(Saz::Event& e)
	{
		Saz::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Saz::MouseButtonPressedEvent>(SAZ_BIND_EVENT_FN(Level::OnMouseButtonPressed));
	}

	bool Level::OnMouseButtonPressed(Saz::MouseButtonPressedEvent& e)
	{
		if (m_GameState == EGameState::Running)
			return false;

		if (e.GetMouseButton() == Saz::Mouse::ButtonLeft)
		{
			Reset();
		}

		return false;
	}

	void Level::Reset()
	{
		m_Player.Reset();
		m_Background.Reset();
		
		for(auto meteor : m_meteors)
		{
			meteor->Reset();
		}

		m_GameState = EGameState::Running;
	}

}