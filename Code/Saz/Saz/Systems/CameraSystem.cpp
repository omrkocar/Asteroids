#include "SazPCH.h"
#include "CameraSystem.h"

#include "Saz/Core/GameTime.h"
#include "Saz/CameraComponent.h"
#include "Saz/InputComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>
#include "NameComponent.h"
#include "WindowResizedOneFrameComponent.h"
#include "Screen.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.inl"
#include "RenderSystem.h"
#include "Core/Application.h"
#include "SceneComponent.h"
#include "imguizmo/ImGuizmo.h"

namespace
{
	float s_TranslateSpeed = 10000.0f;
	constexpr float s_RotateSpeed = 5.0f;
}

namespace ecs
{
	void CameraSystem::Update(const Saz::GameTime& gameTime)
	{		
		auto& registry = m_World->m_Registry;

		const auto& cameraEntity = m_World->GetMainCameraEntity();
		auto& scene = m_World->GetSingleComponent<component::LoadedSceneComponent>();

		if (scene.SceneState != SceneState::Editor || !scene.IsHovered)
			return;

		if (!m_World->IsAlive(cameraEntity))
			return;

		auto& cameraComponent = m_World->GetComponent<component::EditorCameraComponent>(cameraEntity);
		
		if (ImGui::GetIO().MouseWheel != 0.0f)
		{
			cameraComponent.Camera.OnMouseScroll(ImGui::GetIO().MouseWheel);
		}

		if (ImGuizmo::IsUsing())
			return;
		
		auto& inputComponent = m_World->GetComponent<component::InputComponent>(cameraEntity);
		auto& transformComponent = m_World->GetComponent<component::TransformComponent>(cameraEntity);		

		
		const glm::vec2& mouse{ inputComponent.GetMousePosition().x, inputComponent.GetMousePosition().y };
		glm::vec2 delta = { inputComponent.GetMouseDelta().x * 0.003f, inputComponent.GetMouseDelta().y * 0.003f };

		if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonRight))
			cameraComponent.Camera.MousePan(-delta);
		else if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonLeft) && inputComponent.IsKeyHeld(Input::KeyCode::LeftAlt))
			cameraComponent.Camera.MouseRotate(-delta);
		
		cameraComponent.Camera.UpdateView();
	}

	void CameraSystem::ImGuiRender()
	{
	}

	void CameraSystem::OnWindowResized(entt::registry& registry, entt::entity entity)
	{
		if (!m_World->IsAlive(m_World->GetMainCameraEntity()))
			return;

		auto& windowResizeComp = m_World->GetComponent<component::WindowResizedOneFrameComponent>(entity);

		auto& cameraComp = m_World->GetComponent<component::EditorCameraComponent>(m_World->GetMainCameraEntity());
		cameraComp.Camera.SetViewportSize((float)windowResizeComp.Width, (float)windowResizeComp.Height);
	}
}
