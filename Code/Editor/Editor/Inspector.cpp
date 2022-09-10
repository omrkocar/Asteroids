#include "EditorPCH.h"
#include "Inspector.h"

#include "Saz/Core/GameTime.h"
#include <Saz/Screen.h>
#include "Saz/TransformComponent.h"
#include "imgui.h"
#include "Saz/NameComponent.h"
#include "Saz/CameraComponent.h"
#include "Core/String.h"
#include "glm/gtc/type_ptr.inl"
#include "Saz/RenderComponents.h"
#include "imgui_internal.h"

namespace 
{
	static void DrawVec3Control(const String& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.2f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		
		ImGui::Columns(1);

		ImGui::PopID();
	}
}

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
		

		
	}

	void Inspector::ImGuiRender()
	{
		ImGui::Begin("Inspector");

		if (m_WorldOutliner.m_SelectedEntity != entt::null)
			DrawComponents(m_WorldOutliner.m_SelectedEntity);

		ImGui::End();
	}

	void Inspector::DrawComponents(Entity entity)
	{
		DrawNameComponent(entity);
		DrawTransformComponent(entity);
		DrawCameraComponent(entity);
		DrawSpriteComponent(entity);
	}

	void Inspector::DrawNameComponent(Entity entity)
	{
		if (m_World->HasComponent<component::NameComponent>(entity))
		{
			auto& name = m_World->GetComponent<component::NameComponent>(entity).Name;

			ImGui::Text("Name");
			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, name.c_str());
			if (ImGui::InputText(" ", buffer, sizeof(buffer)))
			{
				name = String(buffer);
			}
		}
	}

	void Inspector::DrawTransformComponent(Entity entity)
	{
		if (m_World->HasComponent<component::TransformComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(component::TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component"))
			{
				auto& transform = m_World->GetComponent<component::TransformComponent>(entity);

				DrawVec3Control("Position", transform.Position);
				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transform.Scale, 1.0f);

				ImGui::TreePop();
			}
		}
	}

	void Inspector::DrawCameraComponent(Entity entity)
	{
		if (m_World->HasComponent<component::CameraComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(component::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera Component"))
			{
				auto& cameraComp = m_World->GetComponent<component::CameraComponent>(entity);
				auto& camera = cameraComp.Camera;
				bool isProjection = cameraComp.Camera.GetProjectionType() == Saz::SceneCamera::ProjectionType::Perspective;

				ImGui::Checkbox("Primary", &cameraComp.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)cameraComp.Camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							cameraComp.Camera.SetProjectionType((Saz::SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == Saz::SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float orthoNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetPerspectiveNearClip(orthoNear);

					float orthoFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetPerspectiveFarClip(orthoFar);
				}

				if (camera.GetProjectionType() == Saz::SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComp.FixedAspectRatio);
				}


				ImGui::TreePop();
			}
		}
	}

	void Inspector::DrawSpriteComponent(Entity entity)
	{
		if (m_World->HasComponent<component::SpriteComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(component::SpriteComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Component"))
			{
				auto& spriteComp = m_World->GetComponent<component::SpriteComponent>(entity);

				ImGui::ColorEdit4("Color", glm::value_ptr(spriteComp.Color), 0.1f);

				ImGui::TreePop();
			}
		}
	}

}
