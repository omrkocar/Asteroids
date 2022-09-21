#include "EditorPCH.h"
#include "GameViewport.h"

#include "Saz/Core/GameTime.h"
#include "Saz/NameComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/CameraComponent.h"
#include "Saz/TransformComponent.h"
#include "Saz/SceneComponent.h"

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include "Saz/InputComponent.h"
#include "Saz/Rendering/Framebuffer.h"
#include "Saz/RenderComponents.h"
#include "Saz/WindowResizedOneFrameComponent.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Utils/SceneUtils.h"

namespace ecs
{	
	extern const std::filesystem::path g_DataPath;

	GameViewport::GameViewport()
	{
		
	}

	void GameViewport::Init()
	{
		m_Entity = m_World->CreateEntity();
		
		m_FrameBufferEntity = m_World->CreateEntity();
		auto& frameBufferComp = m_World->AddComponent<component::FrameBufferComponent>(m_FrameBufferEntity);
		Saz::FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { Saz::FrameBufferTextureFormat::RGBA8, Saz::FrameBufferTextureFormat::RED_INTEGER, Saz::FrameBufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		frameBufferComp.FrameBuffer = Saz::FrameBuffer::Create(fbSpec);
		m_FrameBuffer = frameBufferComp.FrameBuffer;

		m_World->m_Registry.on_construct<component::CameraComponent>().connect<&GameViewport::OnCameraComponentAdded>(this);
	}

	void GameViewport::Update(const Saz::GameTime& gameTime)
	{
		SAZ_PROFILE_FUNCTION();

		if (m_World->HasComponent<component::WindowResizedOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::WindowResizedOneFrameComponent>(m_Entity);
		if (m_World->HasComponent<component::LoadSceneRequestOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::LoadSceneRequestOneFrameComponent>(m_Entity);

		if (!m_IsActive)
			return;

		const Saz::FrameBufferSpecification& spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			float width = m_ViewportSize.x;
			float height = m_ViewportSize.y;
			m_FrameBuffer->Resize((uint32_t)width, (uint32_t)height);

			m_World->AddComponent<component::WindowResizedOneFrameComponent>(m_Entity, width, height, WindowType::GameViewport);
		}

		Saz::Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		Saz::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Saz::RenderCommand::Clear();
		m_FrameBuffer->ClearColorAttachment(1, -1);

		RenderScene();

		m_FrameBuffer->Unbind();

		ImGuiRender();
	}

	void GameViewport::RenderScene()
	{
		SAZ_PROFILE_FUNCTION();

		auto& cameraView = m_World->GetAllEntitiesWith<component::CameraComponent>();
		for (auto& cameraEntity : cameraView)
		{
			auto& cameraTransformComp = m_World->GetComponent<component::TransformComponent>(cameraEntity);
			auto& cameraComponent = m_World->GetComponent<component::CameraComponent>(cameraEntity);

			const glm::mat4& cameraTransform = cameraTransformComp.GetTransform();
			Saz::Renderer2D::BeginScene(cameraComponent.Camera, cameraTransform);

			auto view = m_World->GetAllEntitiesWith<component::TransformComponent, component::SpriteComponent>();
			for (auto& entity : view)
			{
				auto& [transform, sprite] = view.get<component::TransformComponent, component::SpriteComponent>(entity);
				Saz::Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			// Draw circles
			{
				auto view = m_World->GetAllEntitiesWith<component::TransformComponent, component::CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<component::TransformComponent, component::CircleRendererComponent>(entity);

					Saz::Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}

			Saz::Renderer2D::EndScene();
		}
	}


	void GameViewport::ImGuiRender()
	{
		DrawViewport();
	}

	void GameViewport::DrawViewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		// Scene
		ImGui::Begin("Game", &m_IsActive);
		DrawOptions();
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewPortHovered = ImGui::IsWindowHovered();
		auto& scene = m_World->GetSingleComponent<component::LoadedSceneComponent>();
		scene.IsFocused = m_ViewportFocused;
		scene.IsHovered = m_ViewPortHovered;

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureId = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				Saz::SceneUtils::OpenScene(*m_World, m_Entity, std::filesystem::path(g_DataPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void GameViewport::DrawOptions()
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 50);
		ImGui::Text("Name");
	}

	void GameViewport::OnCameraComponentAdded(entt::registry& registry, entt::entity entity)
	{
		m_World->AddComponent<component::WindowResizedOneFrameComponent>(m_Entity, m_ViewportSize.x, m_ViewportSize.y, WindowType::GameViewport);
	}
}
