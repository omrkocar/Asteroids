#pragma once
#include "GameObject.h"
#include "glm/glm.hpp"
#include "Saz/Rendering/Texture.h"
#include "Saz/ParticleSystem.h"
#include "Saz/Rendering/OrthographicCamera.h"

namespace Game { class Level; }

namespace Saz
{
	class GameTime;
}

namespace Game
{
	class Player : public GameObject
	{
	public:
		Player(Level* level);
		virtual ~Player();

		void OnUpdate(const Saz::GameTime& gameTime);
		void OnRender(Saz::OrthographicCamera& camera);

		void OnImGuiRender();

		void Reset();
		void Init() override;

	private:
		Saz::ParticleProps m_SmokeParticle, m_EngineParticle;
		Saz::ParticleSystem m_ParticleSystem;

		Level* m_Level;
	};
}