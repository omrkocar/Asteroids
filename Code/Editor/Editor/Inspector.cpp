#include "EditorPCH.h"
#include "Inspector.h"

#include "Core/String.h"

#include "Saz/CameraComponent.h"
#include "Saz/Core/GameTime.h"
#include "Saz/NameComponent.h"
#include "Saz/PhysicsComponents.h"
#include "Saz/RenderComponents.h"
#include "Saz/Rendering/Texture.h"
#include "Saz/SceneComponent.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"

#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.inl>

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
	extern const std::filesystem::path g_DataPath;

	Inspector::Inspector(WorldOutliner& worldOutliner)
		: m_WorldOutliner(worldOutliner)
	{
		
	}

	void Inspector::Update(const Saz::GameTime& gameTime)
	{
		ImGuiRender();
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
		DrawCircleRendererComponent(entity);
		DrawRigidbody2DComponent(entity);
		DrawBoxCollider2DComponent(entity);
		DrawCircleCollider2DComponent(entity);

		ImGui::PopFont();
	}

	void Inspector::DrawAddComponentPopup(Entity entity)
	{
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<component::CameraComponent>("Camera");
			DisplayAddComponentEntry<component::SpriteComponent>("Sprite Renderer");
			DisplayAddComponentEntry<component::CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<component::Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<component::BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<component::CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}
	}

	template<typename T>
	void Inspector::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_World->HasComponent<T>(m_WorldOutliner.m_SelectedEntity))
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_World->AddComponent<T>(m_WorldOutliner.m_SelectedEntity);
				ImGui::CloseCurrentPopup();
			}
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

				//ImGui::Checkbox("Primary", &component.Primary);

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
				
				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_DataPath) / path;
						component.Texture = Saz::Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});
	}

	void Inspector::DrawCircleRendererComponent(Entity entity)
	{
		DrawComponent<component::CircleRendererComponent>(*m_World, "Circle Renderer Component", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.0025f, 0.0f, 1.0f);
			});
	}

	void Inspector::DrawRigidbody2DComponent(Entity entity)
	{
		DrawComponent<component::Rigidbody2DComponent>(*m_World, "Rigidbody2D Component", entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (component::Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			});
	}

	void Inspector::DrawBoxCollider2DComponent(Entity entity)
	{
		DrawComponent<component::BoxCollider2DComponent>(*m_World, "BoxCollider2D Component", entity, [](auto& component)
			{
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

	}

	void Inspector::DrawCircleCollider2DComponent(Entity entity)
	{
		DrawComponent<component::CircleCollider2DComponent>(*m_World, "CircleCollider2D Component", entity, [](auto& component)
			{
				ImGui::DragFloat("Radius", &component.Radius);
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});
	}

}
