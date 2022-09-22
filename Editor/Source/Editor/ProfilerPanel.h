#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class ProfilerPanel final : public System
	{
	public:
		ProfilerPanel();

		virtual void Init() override;

		virtual void Update(const Saz::GameTime& gameTime) override;

		void SetVisible(bool active);
		bool IsActive() { return m_IsActive; }

	private:
		void ImGuiRender();

	private:

		bool m_IsActive = false;
	};
}
