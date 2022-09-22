#include "SazPCH.h"
#include "Renderer.h"

#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Rendering/Shader.h"

namespace Saz
{
	Renderer::SceneData* Renderer::m_SceneData = new SceneData;

	void Renderer::Init()
	{
		SAZ_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetProjection();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}