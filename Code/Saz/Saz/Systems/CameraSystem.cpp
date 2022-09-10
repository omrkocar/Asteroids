#include "SazPCH.h"
#include "CameraSystem.h"

#include "Saz/Core/GameTime.h"
#include "Saz/CameraComponent.h"
#include "Saz/InputComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>
#include "NameComponent.h"
#include "glm/ext/matrix_clip_space.inl"
#include "WindowResizedOneFrameComponent.h"
#include "Screen.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.inl"
#include "RenderSystem.h"
#include "Core/Application.h"

namespace
{
	float s_TranslateSpeed = 2.0f;
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
		m_CameraEntity = m_World->CreateEntity();
		auto& cam = m_World->AddComponent<component::CameraComponent>(m_CameraEntity);
		cam.Camera.SetProjectionType(Saz::SceneCamera::ProjectionType::Perspective);
		m_World->AddComponent<component::NameComponent>(m_CameraEntity).Name = "Main Camera";
		m_World->AddComponent<component::TransformComponent>(m_CameraEntity, glm::vec3(0, 0, 10.0f));

		m_SecondCamera = m_World->CreateEntity();
		auto& cam2 = m_World->AddComponent<component::CameraComponent>(m_SecondCamera);
		cam2.Camera.SetProjectionType(Saz::SceneCamera::ProjectionType::Orthographic);
		cam2.Primary = false;
		m_World->AddComponent<component::NameComponent>(m_SecondCamera).Name = "ClipSpace Camera";
		m_World->AddComponent<component::TransformComponent>(m_SecondCamera, glm::vec3(0, 0, 10.0f));

		auto& registry = m_World->m_Registry;
		registry.on_construct<component::WindowResizedOneFrameComponent>().connect<&CameraSystem::OnWindowResized>(this);
	}

	void CameraSystem::Update(const Saz::GameTime& gameTime)
	{		
		auto& registry = m_World->m_Registry;

		if (Saz::Application::Get().IsViewportFocused() == false)
			return;
		
		const auto cameraView = m_World->GetAllEntitiesWith<component::CameraComponent, component::TransformComponent>();
		for (auto& cameraEntity : cameraView)
		{
			auto& cameraComponent= cameraView.get<component::CameraComponent>(cameraEntity);
			if (cameraComponent.Primary == false)
				continue;

			const auto inputView = m_World->GetAllEntitiesWith<const component::InputComponent>();
			for (auto& inputEntity : inputView)
			{
				const auto& inputComponent = inputView.get<const component::InputComponent>(inputEntity);
				auto& transformComponent = cameraView.get<component::TransformComponent>(cameraEntity);

				if (inputComponent.IsKeyHeld(Input::KeyCode::A))
					transformComponent.Position.x -= s_TranslateSpeed * gameTime.GetDeltaTime();

				if (inputComponent.IsKeyHeld(Input::KeyCode::D))
					transformComponent.Position.x += s_TranslateSpeed * gameTime.GetDeltaTime();

				if (inputComponent.IsKeyHeld(Input::KeyCode::W))
					transformComponent.Position.y += s_TranslateSpeed * gameTime.GetDeltaTime();

				if (inputComponent.IsKeyHeld(Input::KeyCode::S))
					transformComponent.Position.y -= s_TranslateSpeed * gameTime.GetDeltaTime();

				if (inputComponent.IsKeyHeld(Input::KeyCode::Q))
					transformComponent.Rotation -= s_RotateSpeed * gameTime.GetDeltaTime();
				if (inputComponent.IsKeyHeld(Input::KeyCode::E))
					transformComponent.Rotation += s_RotateSpeed * gameTime.GetDeltaTime();
			}
		}
	}

	void CameraSystem::ImGuiRender()
	{
		static bool primaryCamera;
		if (ImGui::Checkbox("Camera A", &primaryCamera))
		{
			m_World->GetComponent<component::CameraComponent>(m_CameraEntity).Primary = primaryCamera;
			m_World->GetComponent<component::CameraComponent>(m_SecondCamera).Primary = !primaryCamera;
		}
	}

	void CameraSystem::OnWindowResized(entt::registry& registry, entt::entity entity)
	{
		auto cameraView = m_World->GetAllEntitiesWith<component::CameraComponent>();
		auto windowResizeComp = m_World->GetComponent<component::WindowResizedOneFrameComponent>(entity);
		for (auto& cameraEntity : cameraView)
		{
			auto& cameraComp = cameraView.get<component::CameraComponent>(cameraEntity);
			if (!cameraComp.FixedAspectRatio)
				cameraComp.Camera.SetViewportSize(windowResizeComp.Width, windowResizeComp.Height);
		}
	}
}
