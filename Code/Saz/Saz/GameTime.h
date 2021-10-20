#pragma once

namespace Saz
{
	class SAZ_API GameTime
	{
	public:
		float m_DeltaTime = 0.f;
		float m_TotalTime = 0.f;
		unsigned int m_Frame = 0;
	};
}