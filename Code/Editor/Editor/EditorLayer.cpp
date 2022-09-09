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
}
