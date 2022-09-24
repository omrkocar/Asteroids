#include "EditorPCH.h"
#include "Inspector.h"

#include <Saz/Components/CameraComponent.h>
#include <Saz/Components/NameComponent.h>
#include <Saz/Components/PhysicsComponents.h>
#include <Saz/Components/RenderComponents.h>
#include <Saz/Components/SceneComponent.h>
#include <Saz/Components/ScriptComponent.h>
#include <Saz/Components/TransformComponent.h>
#include <Saz/Rendering/Texture.h>
#include <Saz/Screen.h>
#include <Saz/Scripting/ScriptEngine.h>

#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.inl>

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(150.0f), func(label, __VA_ARGS__))

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
			uiFunction(component, world);
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
		if (!m_IsActive)
			return;

		ImGuiRender();
	}

	void Inspector::ImGuiRender()
	{
		ImGui::Begin("Inspector", &m_IsActive);

		if (m_WorldOutliner.m_SelectedEntity != entt::null)
		{
			DrawComponents(m_WorldOutliner.m_SelectedEntity);
		}

		ImGui::End();
	}

	void Inspector::DrawComponents(Entity entity)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushFont(boldFont);

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		DrawNameComponent(entity);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		DrawAddComponentPopup(entity);

		ImGui::PushItemWidth(-1);
		DrawTransformComponent(entity);
		DrawCameraComponent(entity);
		DrawScriptComponent(entity);
		DrawSpriteComponent(entity);
		DrawCircleRendererComponent(entity);
		DrawRigidbody2DComponent(entity);
		DrawBoxCollider2DComponent(entity);
		DrawCircleCollider2DComponent(entity);

		ImGui::PopItemWidth();
		ImGui::PopFont();
	}

	void Inspector::DrawAddComponentPopup(Entity entity)
	{
		if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<component::CameraComponent>("Camera");
			DisplayAddComponentEntry<component::ScriptComponent>("Script");
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
		DrawComponent<component::TransformComponent>(*m_World, "Transform Component", entity, [](auto& component, auto& world)
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
		DrawComponent<component::CameraComponent>(*m_World, "Camera Component", entity, [](auto& component, auto& world)
			{
				auto& camera = component.Camera;
				bool isProjection = component.Camera.GetProjectionType() == Saz::SceneCamera::ProjectionType::Perspective;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];

				if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Projection", currentProjectionTypeString))
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
					if (IMGUI_LEFT_LABEL(ImGui::DragFloat,"Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float orthoNear = camera.GetPerspectiveNearClip();
					if (IMGUI_LEFT_LABEL(ImGui::DragFloat, "Near", &orthoNear))
						camera.SetPerspectiveNearClip(orthoNear);

					float orthoFar = camera.GetPerspectiveFarClip();
					if (IMGUI_LEFT_LABEL(ImGui::DragFloat, "Far", &orthoFar))
						camera.SetPerspectiveFarClip(orthoFar);
				}

				if (camera.GetProjectionType() == Saz::SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (IMGUI_LEFT_LABEL(ImGui::DragFloat, "Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (IMGUI_LEFT_LABEL(ImGui::DragFloat, "Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (IMGUI_LEFT_LABEL(ImGui::DragFloat, "Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					IMGUI_LEFT_LABEL(ImGui::Checkbox, "Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});
	}

	void Inspector::DrawScriptComponent(Entity entity)
	{
		DrawComponent<component::ScriptComponent>(*m_World, "Script Component", entity, [entity](auto& component, auto& world) mutable
			{
				bool scriptClassExists = Saz::ScriptEngine::EntityClassExists(component.ClassName);

				static char buffer[64];
				strcpy(buffer, component.ClassName.c_str());

				if (!scriptClassExists)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

				if (IMGUI_LEFT_LABEL(ImGui::InputText, "Class", buffer, sizeof(buffer)))
					component.ClassName = buffer;

				Saz::UUID id = world.GetUUID(entity);
				// Fields
				Saz::Ref<Saz::ScriptInstance> scriptInstance = Saz::ScriptEngine::GetEntityScriptInstance(id);
				if (scriptInstance)
				{
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();

					for (const auto& [name, field] : fields)
					{
						if (field.Type == Saz::ScriptFieldType::Float)
						{
							float data = scriptInstance->GetFieldValue<float>(name);
							if (IMGUI_LEFT_LABEL(ImGui::DragFloat, name.c_str(), &data))
							{
								scriptInstance->SetFieldValue(name, data);
							}
						}
					}
				}

				if (!scriptClassExists)
					ImGui::PopStyleColor();
			});
	}

	void Inspector::DrawSpriteComponent(Entity entity)
	{
		DrawComponent<component::SpriteComponent>(*m_World, "Sprite Component", entity, [](auto& component, auto& world)
			{
				IMGUI_LEFT_LABEL(ImGui::ColorEdit4, "Color", glm::value_ptr(component.Color));
				
				IMGUI_LEFT_LABEL(ImGui::Button,"Texture", ImVec2(100.0f, 0.0f));

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

				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});
	}

	void Inspector::DrawCircleRendererComponent(Entity entity)
	{
		DrawComponent<component::CircleRendererComponent>(*m_World, "Circle Renderer Component", entity, [](auto& component, auto& world)
			{
				IMGUI_LEFT_LABEL(ImGui::ColorEdit4, "Color", glm::value_ptr(component.Color));
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Fade", &component.Fade, 0.0025f, 0.0f, 1.0f);
			});
	}

	void Inspector::DrawRigidbody2DComponent(Entity entity)
	{
		DrawComponent<component::Rigidbody2DComponent>(*m_World, "Rigidbody2D Component", entity, [](auto& component, auto& world)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

				if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (Physics::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				IMGUI_LEFT_LABEL(ImGui::InputFloat, "Gravity Scale", &component.GravityScale);
				IMGUI_LEFT_LABEL(ImGui::Checkbox, "Fixed Rotation", &component.FixedRotation);
			});
	}

	void Inspector::DrawBoxCollider2DComponent(Entity entity)
	{
		DrawComponent<component::BoxCollider2DComponent>(*m_World, "BoxCollider2D Component", entity, [](auto& component, auto& world)
			{
				IMGUI_LEFT_LABEL(ImGui::DragFloat2, "Size", glm::value_ptr(component.Size));
				IMGUI_LEFT_LABEL(ImGui::DragFloat2,"Offset", glm::value_ptr(component.Offset));
				IMGUI_LEFT_LABEL(ImGui::DragFloat,"Density", &component.Density, 0.01f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat,"Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat,"Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat,"RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

	}

	void Inspector::DrawCircleCollider2DComponent(Entity entity)
	{
		DrawComponent<component::CircleCollider2DComponent>(*m_World, "CircleCollider2D Component", entity, [](auto& component, auto& world)
			{
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Radius", &component.Radius);
				IMGUI_LEFT_LABEL(ImGui::DragFloat2, "Offset", glm::value_ptr(component.Offset));
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Density", &component.Density, 0.01f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				IMGUI_LEFT_LABEL(ImGui::DragFloat, "RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});
	}
}
