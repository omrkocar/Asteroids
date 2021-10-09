#pragma once

#include <SFML/System/Time.hpp>

namespace Saz
{
	class GameTime
	{
	public:
		sf::Time m_Time = { };

		float m_DeltaTime = 0.f;
		float m_TotalTime = 0.f;
		unsigned int m_Frame = 0;
	};
}