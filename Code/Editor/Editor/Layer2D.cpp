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
	m_CameraController.OnUpdate(gameTime);

	// Render
	Saz::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Saz::RenderCommand::Clear();

	Saz::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Saz::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.75f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Saz::Renderer2D::DrawQuad({ 0.5f, -0.05f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Saz::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture);
	Saz::Renderer2D::EndScene();

	// TODO: Add Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Saz::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Saz::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);

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
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}
