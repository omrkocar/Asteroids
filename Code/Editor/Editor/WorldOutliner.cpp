#include "EditorPCH.h"
#include "WorldOutliner.h"

#include "Saz/Core/GameTime.h"

#include <entt/entt.hpp>
#include "Saz/NameComponent.h"
#include "Saz/LevelComponent.h"
#include "imgui.h"
#include "Saz/CameraComponent.h"

namespace ecs
{	
	WorldOutliner::WorldOutliner()
	{
		
	}

	void WorldOutliner::Update(const Saz::GameTime& gameTime)
	{
		if (!m_IsActive)
			return;

		//DrawWorldOutliner();
	}

	void WorldOutliner::DrawWorldOutliner()
	{
		
	}

	void WorldOutliner::ImGuiRender()
	{
		if (ImGui::Begin("World Outliner", &m_IsActive, ImGuiWindowFlags_MenuBar))
		{
			static bool objectHandledRightClick = false;
			if (ImGui::BeginListBox("Objects", ImVec2(-1, -1)))
			{
				auto& registry = m_World->m_Registry;
				const auto view = registry.view<component::NameComponent>();

				for (const ecs::Entity& entity : view)
				{
					auto& nameComp = view.get<component::NameComponent>(entity);

					ImGui::PushID(static_cast<int32_t>(entity));
					ImGui::Text(nameComp.Name.c_str());

					if (ImGui::IsItemClicked())
					{
						m_SelectedEntity = entity;
						m_IsObjectInspectorOn = true;
					}

					if (ImGui::BeginPopupContextItem("Object Options"))
					{
						objectHandledRightClick = true;

						ImGui::Text("Rename:");
						String clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
						size_t clipSize = clipText.length();
						const size_t size = 32;
						char newText[size];
						strncpy_s(newText, size, nameComp.Name.c_str(), sizeof(newText));

						/*ImGui::InputText("", newText, size);
						nameComp.Name = newText;*/

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

			if (!objectHandledRightClick)
			{
				if (ImGui::BeginPopupContextItem("Object Selection Menu"))
				{
					if (ImGui::Button("Camera"))
					{
						ecs::Entity entity = m_World->CreateEntity();
						m_World->AddComponent<component::LevelComponent>(entity);
						//auto& cam = m_World->AddComponent<component::CameraComponent>(entity);
						auto& nameComp = m_World->AddComponent<component::NameComponent>(entity);
						nameComp.Name = "Camera";
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}
	}
}
