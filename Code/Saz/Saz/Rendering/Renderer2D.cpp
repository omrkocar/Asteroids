#include "SazPCH.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Core/Core.h"
#include "RenderCommand.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"

namespace Saz
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_RenderData;

	void Renderer2D::Init()
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData = new Renderer2DStorage();
		s_RenderData->VertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Saz::Ref<Saz::VertexBuffer> squareVB;
		squareVB.reset(Saz::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Saz::ShaderDataType::Float3, "a_Position" },
			{ Saz::ShaderDataType::Float2, "a_TexCoord" }
			});
		s_RenderData->VertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(Saz::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_RenderData->VertexArray->SetIndexBuffer(squareIB);

		s_RenderData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_RenderData->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_RenderData->TextureShader = Shader::Create("C:/Dev/SazEngine/Data/Shaders/Texture.glsl");
		s_RenderData->TextureShader->Bind();
		s_RenderData->TextureShader->SetInt("u_Texture", 0);
		s_RenderData->TextureShader->SetFloat("u_TilingFactor", 1.0f);
	}

	void Renderer2D::Shutdown()
	{
		delete s_RenderData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData->TextureShader->Bind();
		s_RenderData->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
	}

	void Renderer2D::EndScene()
	{
		
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData->TextureShader->SetFloat4("u_Color", color);
		s_RenderData->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_RenderData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_RenderData->TextureShader->SetMat4("u_Transform", transform);

		s_RenderData->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_RenderData->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData->TextureShader->SetFloat4("u_Color", tintColor);
		s_RenderData->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_RenderData->TextureShader->SetMat4("u_Transform", transform);

		s_RenderData->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_RenderData->VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData->TextureShader->SetFloat4("u_Color", color);
		s_RenderData->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_RenderData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_RenderData->TextureShader->SetMat4("u_Transform", transform);

		s_RenderData->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_RenderData->VertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData->TextureShader->SetFloat4("u_Color", tintColor);
		s_RenderData->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_RenderData->TextureShader->SetMat4("u_Transform", transform);

		s_RenderData->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_RenderData->VertexArray);
	}

}