#include "EditorPCH.h"
#include "WorldOutliner.h"

#include "Saz/Core/GameTime.h"

#include <entt/entt.hpp>
#include "Saz/NameComponent.h"
#include "Saz/LevelComponent.h"
#include "imgui.h"
#include "Saz/CameraComponent.h"
#include "Saz/TransformComponent.h"

namespace ecs
{	
	WorldOutliner::WorldOutliner()
	{
		
	}

	void WorldOutliner::Update(const Saz::GameTime& gameTime)
	{


		//DrawWorldOutliner();
	}

	void WorldOutliner::DrawWorldOutliner()
	{
		
	}

	void WorldOutliner::ImGuiRender()
	{
		if (!m_IsActive)
			return;

		ImGui::Begin("World Outliner", &m_IsActive, ImGuiWindowFlags_MenuBar);

		auto& registry = m_World->m_Registry;
		const auto view = m_World->GetAllEntitiesWith<component::NameComponent>();
		for (const ecs::Entity& entity : view)
		{
			auto& nameComp = view.get<component::NameComponent>(entity);

			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = entt::null;

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Object"))
			{
				CreateBaseEntity();
			}
			
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void WorldOutliner::DrawEntityNode(Entity entity)
	{
		auto& nameComp = m_World->GetComponent<component::NameComponent>(entity);
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)entity, flags, nameComp.Name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (opened)
			ImGui::TreePop();
	}

	ecs::Entity WorldOutliner::CreateBaseEntity()
	{
		auto entity = m_World->CreateEntity();
		m_World->AddComponent<component::NameComponent>(entity, "Empty Object");
		m_World->AddComponent<component::TransformComponent>(entity);
		return entity;
	}

}
