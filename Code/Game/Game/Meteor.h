#pragma once

#include "glm/glm.hpp"
#include "GameObject.h"
#include "Level.h"

namespace Saz 
{ 
	class Texture2D;
	class GameTime;
}

namespace Game
{
	class Meteor : public GameObject
	{
	public:
		Meteor(Level& level);
		~Meteor();

		virtual void OnUpdate(const Saz::GameTime& gameTime);
		virtual void OnRender();
		virtual void OnImGuiRender();

		void Reset();

		Level& m_Level;
	};
}