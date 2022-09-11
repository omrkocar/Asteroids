#include "SazPCH.h"

#include "Saz/CameraComponent.h"
#include "Saz/Systems/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"

#include "Saz/Core/WindowBase.h"

#include <entt/entt.hpp>
#include "InputComponent.h"
#include "Rendering/Renderer2D.h"
#include "Rendering/RenderCommand.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "imgui.h"
#include "Core/Application.h"
#include "glm/gtc/type_ptr.inl"
#include "NameComponent.h"
#include "glm/ext/matrix_transform.hpp"
#include "WindowResizedOneFrameComponent.h"
#include "SceneSerializer.h"
#include "Utils/PlatformUtils.h"
#include "Rendering/Renderer.h"

namespace ecs
{
	RenderSystem::RenderSystem(Saz::WindowBase& window, CameraSystem& cameraSystem)
		: m_Window(window)
		, m_CameraSystem(cameraSystem)
	{
		
	}

	RenderSystem::~RenderSystem()
	{
		
	}

	void RenderSystem::Init()
	{
		
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		const auto frameBufferView = m_World->GetAllEntitiesWith<component::FrameBufferComponent>();
		const auto cameraView = m_World->GetAllEntitiesWith<component::CameraComponent, component::TransformComponent>();
		for (const auto& frameBufferEntity : frameBufferView)
		{
			const auto& frameBufferComp = registry.get<component::FrameBufferComponent>(frameBufferEntity);

			// Render
			{
				frameBufferComp.FrameBuffer->Bind();
				Saz::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
				Saz::RenderCommand::Clear();
			}

			ecs::Entity mainCameraEntity = m_World->GetMainCameraEntity();
			if (m_World->IsAlive(mainCameraEntity))
			{
				Saz::Renderer2D::ResetStats();

				auto& cameraComponent = m_World->GetComponent<component::CameraComponent>(mainCameraEntity);
				auto& cameraTransformComp = m_World->GetComponent<component::TransformComponent>(mainCameraEntity);

				SAZ_PROFILE_SCOPE("Renderer Draw");

				glm::mat4 transform = cameraTransformComp.GetTransform();
				Saz::Renderer2D::BeginScene(cameraComponent.Camera, transform);

				auto view = m_World->GetAllEntitiesWith<component::TransformComponent, component::SpriteComponent>();
				for (auto entity : view)
				{
					auto& [transform, sprite] = view.get<component::TransformComponent, component::SpriteComponent>(entity);
					Saz::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				}

				Saz::Renderer2D::EndScene();
			}

			frameBufferComp.FrameBuffer->Unbind();
		}
	}
}
