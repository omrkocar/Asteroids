#include "EditorPCH.h"
#include "Layer2D.h"
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Renderer.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Layer2D::Layer2D()
	: Layer("Layer2D")
	, m_CameraController(1280.0f / 720.0f, true)
{

}

void Layer2D::OnAttach()
{
	m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Checkerboard.png");
}

void Layer2D::OnUpdate(const Saz::GameTime& gameTime)
{
	SAZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(gameTime);

	Saz::Renderer2D::ResetStats();
	// Render
	{
		SAZ_PROFILE_SCOPE("Renderer Prep");
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
				Saz::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color );
			}
		}
		Saz::Renderer2D::EndScene();
	}
}

void Layer2D::OnDetach()
{

}

void Layer2D::OnEvent(Saz::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Layer2D::OnImGuiRender()
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
