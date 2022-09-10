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

namespace ecs
{
	RenderSystem::RenderSystem(Saz::WindowBase& window)
		: m_Window(window)
	{
		
	}

	RenderSystem::~RenderSystem()
	{
		
	}

	void RenderSystem::Init()
	{
		Entity m_FrameBufferEntity = m_World->CreateEntity();
		auto& frameBufferComp = m_World->AddComponent<component::FrameBufferComponent>(m_FrameBufferEntity);
		Saz::FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		frameBufferComp.FrameBuffer = Saz::FrameBuffer::Create(fbSpec);
		m_FrameBuffer = frameBufferComp.FrameBuffer;

		auto entity = m_World->CreateEntity();
		m_World->AddComponent<component::TransformComponent>(entity);
		m_World->AddComponent<component::NameComponent>(entity, "Square");
		m_World->AddComponent<component::SpriteRendererComponent>(entity, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto entity2 = m_World->CreateEntity();
		m_World->AddComponent<component::TransformComponent>(entity2, glm::vec3(2.0f, 0.0f, 0.0f), 45.0f);
		m_World->AddComponent<component::NameComponent>(entity2, "Square");
		m_World->AddComponent<component::SpriteRendererComponent>(entity2, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		const auto frameBufferView = registry.view<component::FrameBufferComponent>();
		const auto cameraView = registry.view<component::CameraComponent, component::TransformComponent>();
		for (const auto& frameBufferEntity : frameBufferView)
		{
			const auto& frameBufferComp = registry.get<component::FrameBufferComponent>(frameBufferEntity);

			if (Saz::FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
				m_SceneSize.x > 0.0f && m_SceneSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.Width != m_SceneSize.x || spec.Height != m_SceneSize.y))
			{
				uint32_t width = (uint32_t)m_SceneSize.x;
				uint32_t height = (uint32_t)m_SceneSize.y;
				m_FrameBuffer->Resize(width, height);

				auto& world = Saz::Application::Get().GetWorld();
				ecs::Entity entity = world.CreateEntity();
				world.AddComponent<component::WindowResizedOneFrameComponent>(entity, width, height);
			}

			Saz::Renderer2D::ResetStats();
			// Render
			{
				frameBufferComp.FrameBuffer->Bind();
				SAZ_PROFILE_SCOPE("Renderer Prep");
				Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
				Saz::RenderCommand::Clear();
			}

			{
				static float rotation = 0.0f;
				rotation += gameTime.GetDeltaTime() * 50.0f;

				SAZ_PROFILE_SCOPE("Renderer Draw");
				for (const auto& cameraEntity : cameraView)
				{
					const auto& [cameraComp, transformComp] = registry.get<component::CameraComponent, component::TransformComponent>(cameraEntity);
					if (!cameraComp.Primary)
						continue;


					glm::mat4 transform = glm::translate(glm::mat4(1.0f), transformComp.Position)
						* glm::scale(glm::mat4(1.0f), { transformComp.Scale.x, transformComp.Scale.y, 1.0f });
					Saz::Renderer2D::BeginScene(cameraComp.Camera, transform);

					auto view = registry.view<component::TransformComponent, component::SpriteRendererComponent>();
					for (auto entity : view)
					{
						auto& [transform, sprite] = view.get<component::TransformComponent, component::SpriteRendererComponent>(entity);
						Saz::Renderer2D::DrawQuad(transform.Position, transform.Scale, sprite.Color);
					}

					Saz::Renderer2D::EndScene();
				}				
			}

			frameBufferComp.FrameBuffer->Unbind();
		}
	}

	void RenderSystem::ImGuiRender()
	{
		static bool dockingEnabled = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockingEnabled, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					Saz::Application::Get().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Profiler");
		auto stats = Saz::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats: ");
		ImGui::Text("DrawCalls: %d ", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		
		static char str0[128] = "Hello, world!";
		ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Scene");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewPortHovered = ImGui::IsWindowHovered();
		// if viewport is not hovered and is not focused, block events, otherwise don't block events
		// if viewport is 
		Saz::Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewPortHovered);

		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();
		if (m_SceneSize != *((glm::vec2*)&scenePanelSize))
		{
			m_SceneSize = { scenePanelSize.x, scenePanelSize.y };
		}

		uint32_t textureId = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureId, ImVec2{ m_SceneSize.x, m_SceneSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::End();
	}

}
