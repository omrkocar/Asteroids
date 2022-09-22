#include "SazPCH.h"
#include "CameraSystem.h"

#include "Saz/Components/CameraComponent.h"
#include "Saz/Core/Application.h"
#include "Saz/Components/InputComponent.h"
#include "Saz/Components/TransformComponent.h"
#include "Saz/Components/WindowResizedOneFrameComponent.h"
#include "Saz/Components/SceneComponent.h"

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace ecs
{
	void CameraSystem::Init()
	{
		if (!m_World->IsAlive(m_World->GetMainCameraEntity()))
		{
			m_World->CreateMainCamera();
		}
	}

	void CameraSystem::Update(const Saz::GameTime& gameTime)
	{		
		auto& registry = m_World->m_Registry;

		auto view = m_World->GetAllEntitiesWith<component::WindowResizedOneFrameComponent>();
		for (auto& entity : view)
		{
			OnWindowResized(entity);
		}

		const auto& cameraEntity = m_World->GetMainCameraEntity();
		auto& scene = m_World->GetSingleComponent<component::LoadedSceneComponent>();

		auto& cameraComponent = m_World->GetComponent<component::EditorCameraComponent>(cameraEntity);
		
		if (ImGui::GetIO().MouseWheel != 0.0f && scene.IsHovered)
		{
			cameraComponent.Camera.OnMouseScroll(ImGui::GetIO().MouseWheel);
		}

		if (scene.IsHovered && !ImGuizmo::IsUsing())
		{
			auto& inputComponent = m_World->GetComponent<component::InputComponent>(cameraEntity);
			auto& transformComponent = m_World->GetComponent<component::TransformComponent>(cameraEntity);


			const glm::vec2& mouse{ inputComponent.GetMousePosition().x, inputComponent.GetMousePosition().y };
			glm::vec2 delta = { inputComponent.GetMouseDelta().x * 0.003f, inputComponent.GetMouseDelta().y * 0.003f };
			if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonRight))
				cameraComponent.Camera.MousePan(-delta);
			else if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonLeft) && inputComponent.IsKeyHeld(Input::KeyCode::LeftAlt))
				cameraComponent.Camera.MouseRotate(-delta);
		}

		cameraComponent.Camera.UpdateView();
	}

	void CameraSystem::OnWindowResized(Entity entity)
	{
		auto& windowResizeComp = m_World->GetComponent<component::WindowResizedOneFrameComponent>(entity);

		if (windowResizeComp.WindowType == WindowType::EditorViewport)
		{
			auto& cameraComp = m_World->GetComponent<component::EditorCameraComponent>(m_World->GetMainCameraEntity());
			cameraComp.Camera.SetViewportSize((float)windowResizeComp.Width, (float)windowResizeComp.Height);
		}
		else
		{
			const auto cameraView = m_World->GetAllEntitiesWith<component::CameraComponent>();
			for (const auto& camera : cameraView)
			{
				auto& cameraComp = m_World->m_Registry.get<component::CameraComponent>(camera);
				cameraComp.Camera.SetViewportSize((float)windowResizeComp.Width, (float)windowResizeComp.Height);
			}
		}
	}
}
