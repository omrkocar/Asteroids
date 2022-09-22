#pragma once
#include "Saz/Systems/System.h"

namespace ecs 
{
	class ProjectSettingsWindow; 
	class ProfilerPanel;
	class SceneEditor;
	class GameViewport;
	class Inspector;
	class WorldOutliner;
	class ContentBrowser;
}

namespace component 
{ 
	struct LoadedSceneComponent; 
}

namespace ecs
{
	class MenuBar : public ecs::System
	{
	public:
		MenuBar(
			ProjectSettingsWindow& projectSettings, 
			ProfilerPanel& profilerPanel, 
			SceneEditor& sceneEditor,
			GameViewport& gameViewport,
			Inspector& inspector,
			WorldOutliner& worldOutliner,
			ContentBrowser& contentBrowser);
		~MenuBar();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		virtual void ImGuiRender();

		void DrawEditMenu();
		void DrawFileMenu();
		void DrawWindowsMenu();

		void NewScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		ProfilerPanel& m_ProfilerPanel;
		ProjectSettingsWindow& m_ProjectSettings;
		SceneEditor& m_SceneEditor;
		GameViewport& m_GameViewport;
		Inspector& m_Inspector;
		WorldOutliner& m_WorldOutliner;
		ContentBrowser& m_ContentBrowser;

		Entity m_Entity;
		component::LoadedSceneComponent* m_Scene;

		bool m_isProjectSettingsWindowOpen = false;
	};
}