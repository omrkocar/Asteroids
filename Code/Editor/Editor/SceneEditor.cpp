#include "EditorPCH.h"
#include "SceneEditor.h"

#include "Editor/Application.h"

#include <Saz/Utils/PlatformUtils.h>
#include <Saz/SceneSerializer.h>
#include <Saz/Rendering/Renderer.h>
#include <Saz/Rendering/Renderer2D.h>
#include <Saz/Rendering/Framebuffer.h>
#include <Saz/RenderComponents.h>
#include <Saz/WindowResizedOneFrameComponent.h>
#include <Saz/CameraComponent.h>
#include "Saz/SceneComponent.h"
#include "Saz/InputComponent.h"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include "Saz/TransformComponent.h"
#include "glm/gtc/type_ptr.inl"
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
	}

	void SceneEditor::PreUpdate(const Saz::GameTime& gameTime)
	{
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

				ecs::Entity entity = m_World->CreateEntity();
				m_World->AddComponent<component::WindowResizedOneFrameComponent>(entity, (uint32_t)width, (uint32_t)height);
			}
		}
	}

	void SceneEditor::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		const auto windowResizeView = m_World->GetAllEntitiesWith<const component::WindowResizedOneFrameComponent>();
		for (auto& ent : windowResizeView)
		{
			m_World->DestroyEntity(ent);
		}

		const auto frameBufferView = m_World->GetAllEntitiesWith<component::FrameBufferComponent>();
		for (const auto& frameBufferEntity : frameBufferView)
		{
			const auto& frameBuffer = m_World->m_Registry.get<component::FrameBufferComponent>(frameBufferEntity).FrameBuffer;

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

		ProcessInput();
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
					m_WorldOutliner.m_SelectedEntity = m_HoveredEntity;
			}


			bool control = inputComp.IsKeyHeld(Input::KeyCode::LeftControl) || inputComp.IsKeyHeld(Input::KeyCode::RightControl);
			bool shift = inputComp.IsKeyHeld(Input::KeyCode::LeftShift) || inputComp.IsKeyHeld(Input::KeyCode::RightShift);

			if (!m_ViewportFocused)
				return;

			if (inputComp.IsKeyHeld(Input::KeyCode::S))
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}
			}

			if (inputComp.IsKeyHeld(Input::KeyCode::N))
			{
				if (control)
				{
					NewScene();
				}
			}

			if (inputComp.IsKeyHeld(Input::KeyCode::O))
			{
				if (control)
				{
					OpenScene();
				}
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
		m_World->GetSingleComponent<component::LoadedSceneComponent>().IsFocused = m_ViewportFocused;
		m_World->GetSingleComponent<component::LoadedSceneComponent>().IsHovered = m_ViewPortHovered;

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_SceneSize.x != viewportPanelSize.x || m_SceneSize.y != viewportPanelSize.y)
		{
			m_SceneSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t textureId = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureId, ImVec2{ m_SceneSize.x, m_SceneSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_DataPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_WorldOutliner.m_SelectedEntity;
		if (m_World->IsAlive(selectedEntity))
		{
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
					ImGuizmo::LOCAL,
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

		ImGui::End();
		ImGui::PopStyleVar();

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
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void SceneEditor::NewScene()
	{
		auto entity = m_World->CreateEntity();
		auto& sceneComponent = m_World->AddComponent<component::NewSceneRequestOneFrameComponent>(entity);

		Saz::Renderer::OnWindowResize((uint32_t)m_SceneSize.x, (uint32_t)m_SceneSize.y);
	}

	void SceneEditor::OpenScene()
	{
		const String& path = Saz::FileDialogs::OpenFile("Saz Scene (*.saz)\0*.saz\0");
		if (!path.empty())
		{
			auto entity = m_World->CreateEntity();
			auto& sceneComponent = m_World->AddComponent<component::LoadSceneRequestOneFrameComponent>(entity);
			sceneComponent.Path = path;
			Saz::Renderer::OnWindowResize((uint32_t)m_SceneSize.x, (uint32_t)m_SceneSize.y);
		}
	}

	void SceneEditor::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".saz")
			return;


		auto entity = m_World->CreateEntity();
		auto& sceneComponent = m_World->AddComponent<component::LoadSceneRequestOneFrameComponent>(entity);
		sceneComponent.Path = path.string();

		Saz::Renderer::OnWindowResize((uint32_t)m_SceneSize.x, (uint32_t)m_SceneSize.y);
	}

	void SceneEditor::SaveScene()
	{
		String scenePath = m_World->GetSingleComponent<component::LoadedSceneComponent>().Path;
		if (scenePath.empty())
		{
			scenePath = Saz::FileDialogs::SaveFile("Saz Scene (*.saz)\0*.saz\0", ".saz");
		}

		auto entity = m_World->CreateEntity();
		auto& sceneComponent = m_World->AddComponent<component::SaveSceneRequestOneFrameComponent>(entity);
		sceneComponent.Path = scenePath;
	}

	void SceneEditor::SaveSceneAs()
	{
		const String& path = Saz::FileDialogs::SaveFile("Saz Scene (*.saz)\0*.saz\0", ".saz");
		if (!path.empty())
		{
			auto entity = m_World->CreateEntity();
			auto& sceneComponent = m_World->AddComponent<component::SaveSceneRequestOneFrameComponent>(entity);
			sceneComponent.Path = path;
		}
	}
}