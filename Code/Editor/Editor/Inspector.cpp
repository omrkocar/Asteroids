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
#include "Saz/SceneComponent.h"

namespace
{
	constexpr ImGuiTreeNodeFlags treeNodeFlags =
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_AllowItemOverlap |
		ImGuiTreeNodeFlags_Framed |
		ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_FramePadding;

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
		ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.2f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename Component, typename UIFunction>
	static void DrawComponent(ecs::EntityWorld& world, const String& name, ecs::Entity entity, UIFunction uiFunction)
	{
		if (world.HasComponent<Component>(entity) == false)
			return;

		auto& component = world.GetComponent<Component>(entity);
		ImVec2 contentRegionAvaliable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();

		ImGui::SameLine(contentRegionAvaliable.x - lineHeight * 0.5f);

		if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
			ImGui::OpenPopup("ComponentSettings");

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			world.RemoveComponent<Component>(entity);
	}
}

namespace ecs
{
	Inspector::Inspector(WorldOutliner& worldOutliner)
		: m_WorldOutliner(worldOutliner)
	{
		
	}

	void Inspector::LateUpdate(const Saz::GameTime& gameTime)
	{

	}

	void Inspector::ImGuiRender()
	{
		ImGui::Begin("Inspector");

		if (m_WorldOutliner.m_SelectedEntity != entt::null)
		{
			auto entity = m_WorldOutliner.m_SelectedEntity;
			DrawComponents(entity);
		}

		ImGui::End();
	}

	void Inspector::DrawComponents(Entity entity)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushFont(boldFont);

		DrawNameComponent(entity);

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		DrawAddComponentPopup(entity);
		ImGui::PopItemWidth();

		DrawTransformComponent(entity);
		DrawCameraComponent(entity);
		DrawSpriteComponent(entity);

		ImGui::PopFont();
	}

	void Inspector::DrawAddComponentPopup(Entity entity)
	{
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Transform Component"))
			{
				m_World->AddComponent<component::TransformComponent>(entity);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Camera Component"))
			{
				m_World->AddComponent<component::CameraComponent>(entity);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Component"))
			{
				m_World->AddComponent<component::SpriteComponent>(entity);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void Inspector::DrawNameComponent(Entity entity)
	{
		auto& name = m_World->GetComponent<component::NameComponent>(entity).Name;

		static char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, name.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			name = String(buffer);
		}		
	}

	void Inspector::DrawTransformComponent(Entity entity)
	{
		DrawComponent<component::TransformComponent>(*m_World, "Transform Component", entity, [](auto& component)
			{
				DrawVec3Control("Position", component.Position);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

	}

	void Inspector::DrawCameraComponent(Entity entity)
	{
		DrawComponent<component::CameraComponent>(*m_World, "Camera Component", entity, [](auto& component)
			{
				auto& camera = component.Camera;
				bool isProjection = component.Camera.GetProjectionType() == Saz::SceneCamera::ProjectionType::Perspective;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							component.Camera.SetProjectionType((Saz::SceneCamera::ProjectionType)i);
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

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

	}

	void Inspector::DrawSpriteComponent(Entity entity)
	{

		DrawComponent<component::SpriteComponent>(*m_World, "Sprite Component", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			});

	}
}
