#include "SazPCH.h"
#include "CameraSystem.h"

#include "Saz/GameTime.h"
#include "Saz/CameraComponent.h"
#include "Saz/InputComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>

namespace
{
	constexpr float s_TranslateSpeed = 1000.f;
	constexpr float s_RotateSpeed = 90.f;
}

namespace ecs
{	

	CameraSystem::CameraSystem()
	{
	}

	void CameraSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;
		const auto cameraView = registry.view<component::CameraComponent, component::TransformComponent, component::MovementComponent>();
		for (const auto& entity : cameraView)
		{
			const auto inputView = registry.view<component::InputComponent>();
			for (auto& inputEntity : inputView)
			{
				const auto& inputComponent = inputView.get<component::InputComponent>(inputEntity);

				component::CameraComponent& cameraComponent = cameraView.get<component::CameraComponent>(entity);
				component::TransformComponent& transformComponent = cameraView.get<component::TransformComponent>(entity);
				component::MovementComponent& movementComponent = cameraView.get<component::MovementComponent>(entity);

				auto projMatrix = cameraComponent.m_ProjectionMatrix;
				/*cameraComponent.m_ViewMatrix.CreateLookAtWorld(transformComponent.m_Position, vec3(0.f, 1.f, 0.f), vec3(0, 0, 0));
				cameraComponent.m_ViewMatrix.Inverse();*/
				//transformComponent.m_Position.x += 1.0f;

				vec3 rotate{ 0 };

				const float speed = movementComponent.m_Speed * gameTime.m_DeltaTime;

				vec3& moveAxis = cameraComponent.m_MovementAxes;
				vec2& rotAxis = cameraComponent.m_RotationAxes;

				
				if (inputComponent.IsKeyHeld(Input::EKeyboard::I))
					rotate.y += 1.f;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::K))
					rotate.y += -1.f;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::J))
					rotate.x += 1.f;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::L))
					rotate.x += -1.f;

				vec3& rotation = transformComponent.m_Rotation;
				vec3& position = transformComponent.m_Position;

				rotation += s_RotateSpeed * gameTime.m_DeltaTime * rotate.GetNormalized();
				const vec3 forwardDir{ sin(rotation.y), 0.0f, cos(rotation.y) };
				const vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
				const vec3 upDir{ 0.0f, 1.0f, 0.0f };

				vec3 moveDir{ 0.0f };
				if (inputComponent.IsKeyHeld(Input::EKeyboard::W))
					moveDir += forwardDir;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::A))
					moveDir += -rightDir;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::S))
					moveDir += -forwardDir;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::D))
					moveDir += rightDir;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::Q))
					moveDir += upDir;
				if (inputComponent.IsKeyHeld(Input::EKeyboard::Z))
					moveDir += -upDir;

				position += movementComponent.m_Speed * gameTime.m_DeltaTime * moveDir.GetNormalized();
				
			}
		}
	}
}
