#pragma once
#include "Saz/Rendering/Texture.h"

namespace Saz
{
	class GameTime;
}

namespace Game
{
	class Background
	{
	public:
		Background();
		virtual ~Background();

		void Init();

		void OnUpdate(const Saz::GameTime& gameTime);
		void OnRender();

		void OnImGuiRender();

		void Reset();

	private:
		Saz::Ref<Saz::Texture2D> m_Texture;
		float xPos1, xPos2;
		float m_ParallaxSpeed;
		float m_Size;
	};
}