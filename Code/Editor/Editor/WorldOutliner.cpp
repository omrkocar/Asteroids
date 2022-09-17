#include "EditorPCH.h"
#include "WorldOutliner.h"

#include "Saz/Core/GameTime.h"
#include "Saz/NameComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/CameraComponent.h"
#include "Saz/TransformComponent.h"
#include "Saz/SceneComponent.h"

#include <entt/entt.hpp>
#include <imgui/imgui.h>

namespace ecs
{	
	WorldOutliner::WorldOutliner()
	{
		
	}

	void WorldOutliner::Init()
	{
		m_World->m_Registry.on_construct<component::LoadSceneRequestOneFrameComponent>().connect<&WorldOutliner::OnSceneLoadRequest>(this);
		m_World->m_Registry.on_construct<component::NewSceneRequestOneFrameComponent>().connect<&WorldOutliner::OnSceneLoadRequest>(this);
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
		const auto view = m_World->GetAllEntitiesWith<component::NameComponent, component::SceneEntityComponent>();
		for (const ecs::Entity& entity : view)
		{
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = entt::null;

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Object"))
			{
				auto entity = CreateBaseEntity();
				m_SelectedEntity = entity;
			}
			
			ImGui::EndPopup();
		}

		ImGui::End();
	}


	void WorldOutliner::Update(const Saz::GameTime& gameTime)
	{
		if (m_World->IsAlive(m_EntityToDelete))
		{
			m_World->DestroyEntity(m_EntityToDelete);

			if (m_SelectedEntity == m_EntityToDelete)
				m_SelectedEntity = entt::null;

			m_EntityToDelete = entt::null;
		}
	}

	void WorldOutliner::DrawEntityNode(Entity entity)
	{
		auto& nameComp = m_World->GetComponent<component::NameComponent>(entity);
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)entity, flags, nameComp.Name.c_str());
		if (ImGui::IsItemClicked())
		{
			// Add a one frame component for this
			m_SelectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Object"))
			{
				m_EntityToDelete = entity;
			}

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();
	}

	ecs::Entity WorldOutliner::CreateBaseEntity()
	{
		auto entity = m_World->CreateEntity();
		m_World->AddComponent<component::NameComponent>(entity, "Empty Object");
		m_World->AddComponent<component::TransformComponent>(entity);
		m_World->AddComponent<component::SceneEntityComponent>(entity);
		return entity;
	}

	void WorldOutliner::OnSceneLoadRequest(entt::registry& registry, entt::entity entity)
	{
		m_SelectedEntity = entt::null;
	}
}
