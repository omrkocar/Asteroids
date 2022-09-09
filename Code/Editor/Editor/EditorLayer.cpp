#include "EditorPCH.h"
#include "EditorLayer.h"
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Renderer.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Saz/Rendering/SubTexture2D.h"


static const uint32_t s_MapWidth = 24;

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
	, m_CameraController(1280.0f / 720.0f, true)
{

}

void EditorLayer::OnAttach()
{
	m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Checkerboard.png");

	Saz::FrameBufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_FrameBuffer = Saz::FrameBuffer::Create(fbSpec);
}

void EditorLayer::OnUpdate(const Saz::GameTime& gameTime)
{
	SAZ_PROFILE_FUNCTION();

	if (Saz::FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		m_SceneSize.x > 0.0f && m_SceneSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_SceneSize.x || spec.Height != m_SceneSize.y))
	{
		m_FrameBuffer->Resize((uint32_t)m_SceneSize.x, (uint32_t)m_SceneSize.y);
		m_CameraController.ResizeBounds(m_SceneSize.x, m_SceneSize.y);
	}

	// Update
	if (m_ViewportFocused)
		m_CameraController.OnUpdate(gameTime);

	Saz::Renderer2D::ResetStats();
	// Render
	{
		SAZ_PROFILE_SCOPE("Renderer Prep");
		m_FrameBuffer->Bind();
		Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Saz::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += gameTime.GetDeltaTime() * 50.0f;

		SAZ_PROFILE_SCOPE("Renderer Draw");

		Saz::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Saz::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.75f, 0.75f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Saz::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Saz::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Saz::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), { 20.0f, 20.0f }, m_Texture, 10.0f);
		Saz::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_Texture, 20.0f);
		Saz::Renderer2D::EndScene();

		Saz::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 1.0f };
				Saz::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		Saz::Renderer2D::EndScene();
		m_FrameBuffer->Unbind();
	}
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnEvent(Saz::Event& event)
{
	m_CameraController.OnEvent(event);
}

void EditorLayer::OnImGuiRender()
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

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
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
