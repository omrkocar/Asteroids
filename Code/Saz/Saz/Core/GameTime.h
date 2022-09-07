#pragma once

namespace Saz
{
	class SAZ_API GameTime
	{
		friend class Application;
	public:
		float GetDeltaTime() const { return m_DeltaTime; }
		float GetDeltaMiliseconds() const { return m_DeltaTime * 1000.0f; }
		float GetTime() const { return m_TotalTime; }
		unsigned int GetFrameCount() const { return m_Frame; }

	private:
		float m_DeltaTime = 0.f;
		float m_TotalTime;
		unsigned int m_Frame = 0;
	};
}