#pragma once

#include "glm/glm.hpp"
#include "Background.h"
#include "Saz/Events/MouseEvent.h"
#include "Saz/Events/Event.h"
#include "Game/SpaceGame/Player.h"
#include "Saz/Rendering/OrthographicCamera.h"

namespace Game 
{ 
	class Meteor; 
	class Player;
}

namespace Saz
{
	class GameTime;
}

namespace Game
{
	enum class EGameState
	{
		Running,
		GameOver,
		Stopped,
		None,
	};

	class Level
	{
	public:
		Level();
		virtual ~Level();

		void Init();

		void OnUpdate(const Saz::GameTime& gameTime);
		void OnRender(Saz::OrthographicCamera& camera);

		void OnImGuiRender();
		EGameState& GetGameState() { return m_GameState; }
		Player& GetPlayer() { return m_Player; }
		void OnCollision(const Meteor& meteor);

		void OnEvent(Saz::Event& e);
	private:
		bool OnMouseButtonPressed(Saz::MouseButtonPressedEvent& e);
		void Reset();

	private:
		Player m_Player;
		Background m_Background;
		glm::vec4 m_Color{ 0.5f, 0.2f, 0.8f, 1.0f };
		std::vector<Meteor*> m_meteors;

		EGameState m_GameState = EGameState::Running;
	};
}