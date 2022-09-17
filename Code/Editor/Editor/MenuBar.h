#pragma once
#include "Saz/Systems/System.h"

namespace ecs { class ProjectSettingsWindow; }

namespace component { struct LoadedSceneComponent; }

namespace ecs
{
	class MenuBar : public ecs::System
	{
	public:
		MenuBar(ProjectSettingsWindow& projectSettings);
		~MenuBar();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		virtual void ImGuiRender();

		void DrawEditMenu();

		void DrawFileMenu();

		void DrawProjectSettingsWindow();
		void DrawPhysicsSettings();
		void NewScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		Entity m_Entity;
		component::LoadedSceneComponent* m_Scene;
		ProjectSettingsWindow& m_ProjectSettings;

		bool m_isProjectSettingsWindowOpen = false;
	};
}