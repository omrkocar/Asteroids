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

namespace
{
	float s_TranslateSpeed = 10000.0f;
	constexpr float s_RotateSpeed = 5.0f;
}

namespace ecs
{	

	CameraSystem::CameraSystem()
	{
	}

	void CameraSystem::Destroy()
	{
		
	}

	void CameraSystem::Init()
	{
		auto& registry = m_World->m_Registry;
		registry.on_construct<component::WindowResizedOneFrameComponent>().connect<&CameraSystem::OnWindowResized>(this);
	}

	void CameraSystem::PostInit()
	{
		if (!m_World->IsAlive(m_World->GetMainCameraEntity()))
		{
			m_World->CreateMainCamera();
		}
	}

	void CameraSystem::Update(const Saz::GameTime& gameTime)
	{		
		auto& registry = m_World->m_Registry;

		const auto& cameraEntity = m_World->GetMainCameraEntity();

		if (!m_World->GetSingleComponent<component::LoadedSceneComponent>().IsHovered)
			return;

		if (!m_World->IsAlive(cameraEntity))
			return;

		auto& cameraComponent = m_World->GetComponent<component::EditorCameraComponent>(cameraEntity);
		if (m_World->HasComponent<component::MouseScrollOneFrameComponent>(cameraEntity))
		{
			auto& scrollComponent = m_World->GetComponent<component::MouseScrollOneFrameComponent>(cameraEntity);

			cameraComponent.Camera.OnMouseScroll(scrollComponent.YOffset);
			m_World->RemoveComponent<component::MouseScrollOneFrameComponent>(cameraEntity);
		}
		
		auto& inputComponent = m_World->GetComponent<component::InputComponent>(cameraEntity);
		auto& transformComponent = m_World->GetComponent<component::TransformComponent>(cameraEntity);		

		if (inputComponent.IsKeyHeld(Input::KeyCode::LeftAlt))
		{
			const glm::vec2& mouse{ inputComponent.GetMousePosition().x, inputComponent.GetMousePosition().y };
			glm::vec2 delta = { inputComponent.GetMouseDelta().x * 0.003f, inputComponent.GetMouseDelta().y * 0.003f } ;

			if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonMiddle))
				cameraComponent.Camera.MousePan(delta);
			else if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonLeft))
				cameraComponent.Camera.MouseRotate(-delta);
			else if (inputComponent.IsKeyHeld(Input::MouseCode::ButtonRight))
				cameraComponent.Camera.MouseZoom(delta.y);
		}


		cameraComponent.Camera.UpdateView();
	}

	void CameraSystem::ImGuiRender()
	{
	}

	void CameraSystem::OnWindowResized(entt::registry& registry, entt::entity entity)
	{
		auto windowResizeComp = m_World->GetComponent<component::WindowResizedOneFrameComponent>(entity);

		auto& cameraComp = m_World->GetComponent<component::EditorCameraComponent>(m_World->GetMainCameraEntity());
		cameraComp.Camera.SetViewportSize(windowResizeComp.Width, windowResizeComp.Height);
	}
}
