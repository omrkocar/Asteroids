#include "EditorPCH.h"
#include "Toolbar.h"

#include "Saz/Core/GameTime.h"
#include "Saz/NameComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/CameraComponent.h"
#include "Saz/TransformComponent.h"
#include "Saz/SceneComponent.h"

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include "Saz/InputComponent.h"
#include "Saz/Rendering/Texture.h"

namespace ecs
{	
	Toolbar::Toolbar()
	{
		
	}

	void Toolbar::Init()
	{
		auto& registry = m_World->m_Registry;
		m_Entity = m_World->CreateEntity();

		m_PlayIcon = Saz::Texture2D::Create("../../Data/Textures/PlayButton.png");
		m_StopIcon = Saz::Texture2D::Create("../../Data/Textures/StopButton.png");
	}

	void Toolbar::Update(const Saz::GameTime& gameTime)
	{
		if (m_World->HasComponent<component::SceneStateChangeRequestOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::SceneStateChangeRequestOneFrameComponent>(m_Entity);

		ImGuiRender();
	}

	void Toolbar::ImGuiRender()
	{
		DrawToolbar();
	}

	void Toolbar::DrawToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		auto& scene = m_World->GetSingleComponent<component::LoadedSceneComponent>();
		float size = ImGui::GetWindowHeight() - 4.0f;
		m_Icon = scene.SceneState == SceneState::Editor ? m_PlayIcon : m_StopIcon;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)m_Icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (scene.SceneState == SceneState::Editor)
				OnScenePlay();
			else if (scene.SceneState == SceneState::Play)
				OnSceneStop();
		}

		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
	}

	void Toolbar::OnScenePlay()
	{
		m_World->AddComponent<component::SceneStateChangeRequestOneFrameComponent>(m_Entity, SceneState::Play);
	}

	void Toolbar::OnSceneStop()
	{
		m_World->AddComponent<component::SceneStateChangeRequestOneFrameComponent>(m_Entity, SceneState::Editor);
	}
}
