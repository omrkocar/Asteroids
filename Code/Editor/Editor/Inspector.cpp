#include "EditorPCH.h"
#include "Inspector.h"

#include "Saz/Core/GameTime.h"
#include <Saz/Screen.h>
#include "Saz/MovementComponent.h"
#include "Saz/TransformComponent.h"

namespace ecs
{	
	Inspector::Inspector(WorldOutliner& worldOutliner)
		: m_WorldOutliner(worldOutliner)
	{
		
	}

	void Inspector::Init()
	{

	}

	void Inspector::Update(const Saz::GameTime& gameTime)
	{
		//if (m_WorldOutliner.m_IsObjectInspectorOn)
		//{
		//	ImGui::Begin("Inspector");

		//	ecs::Entity& entity = m_WorldOutliner.m_SelectedEntity;
		//	if (entity != entt::null)
		//	{
		//		ImGui::PushID(&entity);
		//		// test hack
		//		if (m_World->HasComponent<component::TransformComponent>(entity))
		//		{
		//			auto& transformComp = m_World->GetComponent<component::TransformComponent>(entity);
		//			transformComp.AddInspector();
		//		}
		//		ImGui::PopID();
		//	}
		//	else
		//	{
		//		ImGui::Text("Select an object to inspect...");
		//	}

		//	ImGui::End();
		//}

		
	}

	void Inspector::DrawObjectInspector()
	{
		ImGui::BeginGroup();

		
	}
}
