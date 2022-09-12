#include "SazPCH.h"
#include "RenderSystem.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/TransformComponent.h"

#include "Saz/Core/WindowBase.h"

#include "Rendering/Renderer2D.h"
#include "Rendering/RenderCommand.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
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
		for (const auto& frameBufferEntity : frameBufferView)
		{
			const auto& frameBuffer = registry.get<component::FrameBufferComponent>(frameBufferEntity).FrameBuffer;

			Saz::Renderer2D::ResetStats();
			frameBuffer->Bind();
			Saz::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
			Saz::RenderCommand::Clear();
			frameBuffer->ClearColorAttachment(1, -1);

			ecs::Entity mainCameraEntity = m_World->GetMainCameraEntity();
			if (m_World->IsAlive(mainCameraEntity))
			{
				auto& cameraComponent = m_World->GetComponent<component::EditorCameraComponent>(mainCameraEntity);
				auto& cameraTransformComp = m_World->GetComponent<component::TransformComponent>(mainCameraEntity);

				SAZ_PROFILE_SCOPE("Renderer Draw");

				glm::mat4 transform = cameraTransformComp.GetTransform();
				Saz::Renderer2D::BeginScene(cameraComponent.Camera);

				auto view = m_World->GetAllEntitiesWith<component::TransformComponent, component::SpriteComponent>();
				for (auto entity : view)
				{
					auto& [transform, sprite] = view.get<component::TransformComponent, component::SpriteComponent>(entity);
					Saz::Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}

				Saz::Renderer2D::EndScene();
			}
		}
	}

	void RenderSystem::LateUpdate(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		const auto frameBufferView = m_World->GetAllEntitiesWith<component::FrameBufferComponent>();
		for (const auto& frameBufferEntity : frameBufferView)
		{
			const auto& frameBufferComp = registry.get<component::FrameBufferComponent>(frameBufferEntity);

			// Render
			frameBufferComp.FrameBuffer->Unbind();
		}
	}

}
