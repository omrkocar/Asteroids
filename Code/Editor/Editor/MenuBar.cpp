#include "EditorPCH.h"
#include "MenuBar.h"

#include "Editor/Application.h"
#include "Editor/ProjectSettingsWindow.h"
#include "Editor/ProfilerPanel.h"

#include "Saz/InputComponent.h"
#include "Saz/SceneComponent.h"
#include "Saz/Utils/PlatformUtils.h"
#include "Saz/Utils/SceneUtils.h"

#include <imgui/imgui.h>

namespace ecs
{
	MenuBar::MenuBar(ProjectSettingsWindow& projectSettings, ProfilerPanel& profilerPanel)
		: m_ProjectSettings(projectSettings)
		, m_ProfilerPanel(profilerPanel)
	{

	}

	MenuBar::~MenuBar()
	{

	}

	void MenuBar::Init()
	{
		m_Scene = &m_World->GetSingleComponent<component::LoadedSceneComponent>();
		m_Entity = m_World->CreateEntity();
	}

	void MenuBar::Update(const Saz::GameTime& gameTime)
	{
		if (m_World->HasComponent<component::LoadSceneRequestOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::LoadSceneRequestOneFrameComponent>(m_Entity);
		if (m_World->HasComponent<component::SaveSceneRequestOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::SaveSceneRequestOneFrameComponent>(m_Entity);
		if (m_World->HasComponent<component::NewSceneRequestOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::NewSceneRequestOneFrameComponent>(m_Entity);

		const auto inputView = m_World->GetAllEntitiesWith<component::InputComponent>();
		for (const auto& inputEntity : inputView)
		{
			const auto& inputComp = m_World->m_Registry.get<component::InputComponent>(inputEntity);

			bool control = inputComp.IsKeyHeld(Input::KeyCode::LeftControl) || inputComp.IsKeyHeld(Input::KeyCode::RightControl);
			bool shift = inputComp.IsKeyHeld(Input::KeyCode::LeftShift) || inputComp.IsKeyHeld(Input::KeyCode::RightShift);

			if (inputComp.IsKeyPressed(Input::KeyCode::S))
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}
			}

			if (inputComp.IsKeyPressed(Input::KeyCode::N))
			{
				if (control)
				{
					NewScene();
				}
			}

			if (inputComp.IsKeyPressed(Input::KeyCode::O))
			{
				if (control)
				{
					Saz::SceneUtils::OpenScene(*m_World, m_Entity);
				}
			}
		}

		ImGuiRender();
	}

	void MenuBar::ImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 4.0f));
		if (ImGui::BeginMenuBar())
		{
			ImGui::PopStyleVar();
			DrawFileMenu();
			DrawEditMenu();

			ImGui::EndMenuBar();
		}
	}

	void MenuBar::DrawEditMenu()
	{
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Project Settings..."))
			{
				m_ProjectSettings.SetVisible(true);
			}

			if (ImGui::MenuItem("Profiler Panel..."))
			{
				m_ProfilerPanel.SetVisible(true);
			}

			ImGui::EndMenu();
		}
	}

	void MenuBar::DrawFileMenu()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				NewScene();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				Saz::SceneUtils::OpenScene(*m_World, m_Entity);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				SaveScene();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				SaveSceneAs();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				Application::Get().Close();
			}

			ImGui::EndMenu();
		}
	}

	void MenuBar::DrawProjectSettingsWindow()
	{
		
	}

	void MenuBar::DrawPhysicsSettings()
	{
	}

	void MenuBar::NewScene()
	{
		m_World->AddComponent<component::NewSceneRequestOneFrameComponent>(m_Entity);
	}

	void MenuBar::SaveScene()
	{
		String scenePath = m_World->GetSingleComponent<component::LoadedSceneComponent>().Path;
		if (scenePath.empty())
		{
			scenePath = Saz::FileDialogs::SaveFile("Saz Scene (*.saz)\0*.saz\0", ".saz");
		}

		auto& sceneComponent = m_World->AddComponent<component::SaveSceneRequestOneFrameComponent>(m_Entity);
		sceneComponent.Path = scenePath;
	}

	void MenuBar::SaveSceneAs()
	{
		const String& path = Saz::FileDialogs::SaveFile("Saz Scene (*.saz)\0*.saz\0", ".saz");
		if (!path.empty())
		{
			auto& sceneComponent = m_World->AddComponent<component::SaveSceneRequestOneFrameComponent>(m_Entity);
			sceneComponent.Path = path;
		}
	}
}