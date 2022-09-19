#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class ProjectSettingsWindow final : public System
	{
	public:
		
		ProjectSettingsWindow();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		void ImGuiRender();

		bool IsVisible() { return m_IsActive; }
		void SetVisible(bool active) { m_IsActive = active; }
		void DrawPhysicsSettings();
	private:
		bool m_IsActive = false;
	};
}
