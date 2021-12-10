#include "GamePCH.h"
#include "WorldOutliner.h"

#include "Saz/GameTime.h"

#include <entt/entt.hpp>
#include "Saz/NameComponent.h"
#include "Saz/LevelComponent.h"
#include "imgui.h"

namespace ecs
{	
	WorldOutliner::WorldOutliner()
	{
		
	}

	void WorldOutliner::Update(const Saz::GameTime& gameTime)
	{
		{
			//ImGui::Begin("Object List", &m_IsActive);

			//ImGui::Separator();

			//ImGui::Text("Objects:");

			//// Arranges Objects vector into a selectable list.
			//bool objectHandledRightClick = false;
			//static int item_current_idx = 0;
			//if (ImGui::BeginListBox("Objects", ImVec2(-1, -1)))
			//{
			//	int n = 0;

			//	auto& registry = m_World->m_Registry;

			//	const auto view = registry.view<component::LevelComponent>();
			//	for (const ecs::Entity& entity : view)
			//	{
			//		const auto nameView = registry.view<component::NameComponent>();
			//		for (const ecs::Entity& nameEnt : nameView)
			//		{
			//			auto& nameComp = nameView.get<component::NameComponent>(nameEnt);
			//			//ImGui::Text(nameComp.m_Name.c_str());
			//			if (ImGui::BeginDragDropSource())
			//			{
			//				//ImGui::SetDragDropPayload("Object", &index, sizeof(int));

			//				//ImGui::EndDragDropSource();
			//			}
			//		}


			//	}

			//	ImGui::EndListBox();
			//}

			//ImGui::End();
		}
		if (!m_IsActive)
			return;

		if (ImGui::Begin("World Outliner", &m_IsActive, ImGuiWindowFlags_MenuBar))
		{
			bool objectHandledRightClick = false;
			if (ImGui::BeginListBox("Objects", ImVec2(-1, -1)))
			{
				auto& registry = m_World->m_Registry;
				const auto view = registry.view<component::LevelComponent, component::NameComponent>();

				for (const ecs::Entity& entity : view)
				{
					auto& nameComp = view.get<component::NameComponent>(entity);

					ImGui::PushID(&entity);
					ImGui::Text(nameComp.m_Name.c_str());

					if (ImGui::BeginPopupContextItem("Object Options"))
					{
						objectHandledRightClick = true;

						ImGui::Text("Rename:");
						String clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
						size_t clipSize = clipText.length();
						const size_t size = 32;
						char newText[size];
						strncpy_s(newText, size, nameComp.m_Name.c_str(), sizeof(newText));

						ImGui::InputText("", newText, size);
						nameComp.m_Name = newText;

						ImGui::Separator();

						if (ImGui::Button("Delete object"))
						{
							m_World->m_Registry.destroy(entity);
						}

						ImGui::EndPopup();
					}
					ImGui::PopID();
				}
				
				ImGui::EndListBox();
			}

			ImGui::End();
		}
	}
}
