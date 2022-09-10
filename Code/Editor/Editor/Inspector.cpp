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


		if (m_World->HasComponent<component::TransformComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(component::TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component"))
			{
				auto& transform = m_World->GetComponent<component::TransformComponent>(entity);

				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
				ImGui::DragFloat("Rotation", &transform.Rotation, 0.1f);
				ImGui::DragFloat2("Scale", glm::value_ptr(transform.Scale), 0.1f);


				ImGui::TreePop();
			}
		}

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
}
