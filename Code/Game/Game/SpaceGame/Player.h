#pragma once
#include "GameObject.h"
#include "glm/glm.hpp"
#include "Saz/Rendering/Texture.h"

namespace Saz
{
	class GameTime;
}

namespace Game
{
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		void OnUpdate(const Saz::GameTime& gameTime);
		void OnRender();

		void OnImGuiRender();

		void Reset();
		void Init() override;

	private:
	};
}