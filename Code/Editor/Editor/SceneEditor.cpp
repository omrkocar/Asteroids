#include "EditorPCH.h"
#include "SceneEditor.h"

#include "Editor/Application.h"

#include "Saz/InputComponent.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Texture.h"
#include "Saz/SceneComponent.h"
#include "Saz/TransformComponent.h"
#include <Saz/CameraComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/Rendering/Framebuffer.h>
#include <Saz/Rendering/Renderer.h>
#include <Saz/Rendering/Renderer2D.h>
#include <Saz/SceneSerializer.h>
#include <Saz/Utils/PlatformUtils.h>
#include <Saz/WindowResizedOneFrameComponent.h>

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.inl>
#include <filesystem>


namespace ecs
{
	extern const std::filesystem::path g_DataPath;

	SceneEditor::SceneEditor(WorldOutliner& worldOutliner)
		: m_WorldOutliner(worldOutliner)
	{
		
	}

	void SceneEditor::Init()
	{
		Entity m_FrameBufferEntity = m_World->CreateEntity();
		auto& frameBufferComp = m_World->AddComponent<component::FrameBufferComponent>(m_FrameBufferEntity);
		Saz::FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { Saz::FrameBufferTextureFormat::RGBA8, Saz::FrameBufferTextureFormat::RED_INTEGER, Saz::FrameBufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		frameBufferComp.FrameBuffer = Saz::FrameBuffer::Create(fbSpec);
		m_FrameBuffer = frameBufferComp.FrameBuffer;

		auto& registry = m_World->m_Registry;
		registry.on_destroy<component::LoadSceneRequestOneFrameComponent>().connect<&SceneEditor::OnLevelLoaded>(this);
		registry.on_construct<component::NewSceneRequestOneFrameComponent>().connect<&SceneEditor::OnLevelLoaded>(this);

		m_PlayIcon = Saz::Texture2D::Create("../../Data/Textures/PlayButton.png");
		m_StopIcon = Saz::Texture2D::Create("../../Data/Textures/StopButton.png");

		m_Entity = m_World->CreateEntity();
	}

	void SceneEditor::PostInit()
	{
		m_Scene = &m_World->GetSingleComponent<component::LoadedSceneComponent>();
	}

	void SceneEditor::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		if (m_World->HasComponent<component::WindowResizedOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::WindowResizedOneFrameComponent>(m_Entity);
		if (m_World->HasComponent<component::SceneStateChangedOneFrameComponent>(m_Entity))
			m_World->RemoveComponent<component::SceneStateChangedOneFrameComponent>(m_Entity);

		const auto frameBufferView = m_World->GetAllEntitiesWith<component::FrameBufferComponent>();
		for (const auto& frameBufferEntity : frameBufferView)
		{
			const auto& frameBuffer = m_World->m_Registry.get<component::FrameBufferComponent>(frameBufferEntity).FrameBuffer;

			const Saz::FrameBufferSpecification& spec = frameBuffer->GetSpecification();
			if (m_SceneSize.x > 0.0f && m_SceneSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.Width != m_SceneSize.x || spec.Height != m_SceneSize.y))
			{
				uint32_t width = (uint32_t)m_SceneSize.x;
				uint32_t height = (uint32_t)m_SceneSize.y;
				frameBuffer->Resize(width, height);

				m_World->AddComponent<component::WindowResizedOneFrameComponent>(m_Entity, (uint32_t)width, (uint32_t)height);
			}

			Saz::Renderer2D::ResetStats();
			frameBuffer->Bind();
			Saz::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
			Saz::RenderCommand::Clear();
			frameBuffer->ClearColorAttachment(1, -1);

			if (m_Scene->SceneState == SceneState::Editor)
				RenderScene();
			else
				RenderRuntime();

			ProcessMousePicking(frameBuffer);

			frameBuffer->Unbind();
		}

		ProcessInput();
	}

	void SceneEditor::ProcessMousePicking(Saz::Ref<Saz::FrameBuffer> frameBuffer)
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = frameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData != -1 ? (entt::entity)pixelData : entt::null;
		}
	}

	void SceneEditor::RenderScene()
	{
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

	void SceneEditor::RenderRuntime()
	{
		auto& cameraView = m_World->GetAllEntitiesWith<component::CameraComponent>();
		for (auto& cameraEntity : cameraView)
		{
			auto& cameraTransformComp = m_World->GetComponent<component::TransformComponent>(cameraEntity);
			auto& cameraComponent = m_World->GetComponent<component::CameraComponent>(cameraEntity);

			SAZ_PROFILE_SCOPE("Renderer Draw");

			glm::mat4 cameraTransform = cameraTransformComp.GetTransform();
			Saz::Renderer2D::BeginScene(cameraComponent.Camera, cameraTransform);

			auto view = m_World->GetAllEntitiesWith<component::TransformComponent, component::SpriteComponent>();
			for (auto& entity : view)
			{
				auto& [transform, sprite] = view.get<component::TransformComponent, component::SpriteComponent>(entity);
				Saz::Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Saz::Renderer2D::EndScene();
		}
	}

	void SceneEditor::ProcessInput()
	{
		const auto inputView = m_World->GetAllEntitiesWith<component::InputComponent>();
		for (const auto& inputEntity : inputView)
		{
			const auto& inputComp = m_World->m_Registry.get<component::InputComponent>(inputEntity);

			if (inputComp.IsKeyPressed(Input::MouseCode::ButtonLeft))
			{
				if (m_ViewPortHovered && !ImGuizmo::IsOver() && !inputComp.IsKeyHeld(Input::KeyCode::LeftAlt))
				{
					m_WorldOutliner.m_SelectedEntity = m_HoveredEntity;
				}
			}

			if (!m_ViewportFocused)
			{
				m_GizmoType = -1;
				return;
			}

			if (inputComp.IsKeyPressed(Input::KeyCode::Q))
				m_GizmoType = -1;
			if (inputComp.IsKeyPressed(Input::KeyCode::W))
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

			if (inputComp.IsKeyPressed(Input::KeyCode::E))
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;

			if (inputComp.IsKeyPressed(Input::KeyCode::R))
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
	}

	void SceneEditor::ImGuiRender()
	{
		DrawScene();
		DrawToolbar();
		DrawMenuBar();
		DrawProfiler();
	}

	void SceneEditor::DrawScene()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		// Scene
		ImGui::Begin("Scene");
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
		m_SceneSize = { viewportPanelSize.x, viewportPanelSize.y };
		

		uint32_t textureId = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureId, ImVec2{ m_SceneSize.x, m_SceneSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				//OpenScene(std::filesystem::path(g_DataPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		DrawGizmos();

		ImGui::End();
		ImGui::PopStyleVar();

	}

	void SceneEditor::DrawGizmos()
	{
		Entity selectedEntity = m_WorldOutliner.m_SelectedEntity;
		if (!m_World->IsAlive(selectedEntity))
			return;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

		auto cameraEntity = m_World->GetMainCameraEntity();
		if (m_World->IsAlive(cameraEntity))
		{
			const auto& camera = m_World->GetComponent<component::EditorCameraComponent>(cameraEntity).Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = camera.GetViewMatrix();

			auto& tc = m_World->GetComponent<component::TransformComponent>(selectedEntity);
			glm::mat4 transform = tc.GetTransform();

			const auto& inputComponent = m_World->GetComponent<component::InputComponent>(cameraEntity);
			bool snap = inputComponent.IsKeyHeld(Input::KeyCode::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(
				glm::value_ptr(cameraView),
				glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType,
				ImGuizmo::WORLD,
				glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);
				tc.Position = position;
				tc.Rotation = rotation;
				tc.Scale = scale;

			}
		}
	}

	void SceneEditor::DrawToolbar()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Saz::Ref<Saz::Texture2D> icon = m_Scene->SceneState == SceneState::Editor ? m_PlayIcon : m_StopIcon;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_Scene->SceneState == SceneState::Editor)
				OnScenePlay();
			else if (m_Scene->SceneState == SceneState::Play)
				OnSceneStop();
		}

		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
	}

	void SceneEditor::OnScenePlay()
	{
		auto& sceneStateComp = m_World->AddComponent<component::SceneStateChangedOneFrameComponent>(m_Entity, SceneState::Play);
		m_Scene->SceneState = SceneState::Play;
	}

	void SceneEditor::OnSceneStop()
	{
		auto& sceneStateComp = m_World->AddComponent<component::SceneStateChangedOneFrameComponent>(m_Entity, SceneState::Editor);
		m_Scene->SceneState = SceneState::Editor;
	}

	void SceneEditor::DrawProfiler()
	{
		ImGui::Begin("Profiler");
		auto stats = Saz::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats: ");
		ImGui::Text("DrawCalls: %d ", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}

	void SceneEditor::DrawMenuBar()
	{
		
	}

	void SceneEditor::OnLevelLoaded(entt::registry& registry, entt::entity entity)
	{
		m_World->AddComponent<component::WindowResizedOneFrameComponent>(m_Entity, (uint32_t)m_SceneSize.x, (uint32_t)m_SceneSize.y);
		if (m_Scene->SceneState != SceneState::Editor)
		{
			auto& sceneStateComp = m_World->AddComponent<component::SceneStateChangedOneFrameComponent>(m_Entity, SceneState::Editor);
			m_Scene->SceneState = SceneState::Editor;
		}

		Saz::Renderer::OnWindowResize((uint32_t)m_SceneSize.x, (uint32_t)m_SceneSize.y);
	}
}