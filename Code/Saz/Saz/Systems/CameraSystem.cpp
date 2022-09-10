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

namespace
{
	float s_TranslateSpeed = 2.0f;
	constexpr float s_RotateSpeed = 350.0f;
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
		m_World->AddComponent<component::CameraComponent>(m_CameraEntity);
		m_World->AddComponent<component::NameComponent>(m_CameraEntity).Name = "Main Camera";
		m_World->AddComponent<component::TransformComponent>(m_CameraEntity);

		m_SecondCamera = m_World->CreateEntity();
		m_World->AddComponent<component::NameComponent>(m_SecondCamera).Name = "Clip Space Camera";
		auto& cc = m_World->AddComponent<component::CameraComponent>(m_SecondCamera);
		m_World->AddComponent<component::TransformComponent>(m_SecondCamera);
		cc.Primary = false;

		auto& registry = m_World->m_Registry;
		registry.on_construct<component::WindowResizedOneFrameComponent>().connect<&CameraSystem::OnWindowResized>(this);
	}

	void CameraSystem::Update(const Saz::GameTime& gameTime)
	{		
		auto& registry = m_World->m_Registry;

		const auto windowResizeView = registry.view<const component::WindowResizedOneFrameComponent>();
		for (auto& ent : windowResizeView)
		{
			m_World->DestroyEntity(ent);
		}

		
		const auto cameraView = registry.view<const component::CameraComponent, component::TransformComponent>();
		for (auto& cameraEntity : cameraView)
		{
			const auto inputView = registry.view<const component::InputComponent>();
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
			}
		}
	}

	void CameraSystem::ImGuiRender()
	{
		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_World->GetComponent<component::TransformComponent>(m_CameraEntity).Position));
		ImGui::DragFloat("Camera speed", &s_TranslateSpeed, 0.1f);
	}

	void CameraSystem::OnWindowResized(entt::registry& registry, entt::entity entity)
	{
		auto cameraView = registry.view<component::CameraComponent>();
		for (auto& cameraEntity : cameraView)
		{
			auto& cameraComp = cameraView.get<component::CameraComponent>(cameraEntity);
			cameraComp.Camera.SetViewportSize(Screen::width, Screen::height);
		}
	}
}
