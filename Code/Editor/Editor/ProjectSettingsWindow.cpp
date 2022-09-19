#include "EditorPCH.h"
#include "ProjectSettingsWindow.h"

#include "Saz/Core/GameTime.h"

#include <imgui/imgui.h>
#include "Saz/ProjectSettings/PhysicsSettings.h"

namespace ecs
{	
	ProjectSettingsWindow::ProjectSettingsWindow()
	{

	}

	void ProjectSettingsWindow::Init()
	{

	}

	void ProjectSettingsWindow::Update(const Saz::GameTime& gameTime)
	{
		ImGuiRender();
	}

	void ProjectSettingsWindow::ImGuiRender()
	{
		if (!m_IsActive)
			return;

		ImGui::Begin("Project Settings", &m_IsActive);

		DrawPhysicsSettings();

		ImGui::End();
	}

	void ProjectSettingsWindow::DrawPhysicsSettings()
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader;
		if (ImGui::TreeNodeEx("Physics", flags))
		{
			static float a = -9.81f;
			ImGui::InputFloat2("Gravity", PhysicsSettings::Gravity, "%.2f");
		}
	}
}
