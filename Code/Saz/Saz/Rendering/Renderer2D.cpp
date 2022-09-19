#include "SazPCH.h"
#include "Saz/Rendering/Renderer2D.h"

#include "Saz/Rendering/VertexArray.h"
#include "Saz/Rendering/Shader.h"
#include "Saz/Rendering/UniformBuffer.h"
#include "Saz/Rendering/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SceneCamera.h"

namespace Saz {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_RenderData;

	void Renderer2D::Init()
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData.QuadVertexArray = VertexArray::Create();

		s_RenderData.QuadVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(QuadVertex));
		s_RenderData.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		s_RenderData.QuadVertexArray->AddVertexBuffer(s_RenderData.QuadVertexBuffer);

		s_RenderData.QuadVertexBufferBase = new QuadVertex[s_RenderData.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_RenderData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_RenderData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_RenderData.MaxIndices);
		s_RenderData.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Circles
		s_RenderData.CircleVertexArray = VertexArray::Create();

		s_RenderData.CircleVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(CircleVertex));
		s_RenderData.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		s_RenderData.CircleVertexArray->AddVertexBuffer(s_RenderData.CircleVertexBuffer);
		s_RenderData.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_RenderData.CircleVertexBufferBase = new CircleVertex[s_RenderData.MaxVertices];

		// Lines
		s_RenderData.LineVertexArray = VertexArray::Create();

		s_RenderData.LineVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(LineVertex));
		s_RenderData.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_RenderData.LineVertexArray->AddVertexBuffer(s_RenderData.LineVertexBuffer);
		s_RenderData.LineVertexBufferBase = new LineVertex[s_RenderData.MaxVertices];

		s_RenderData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_RenderData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_RenderData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_RenderData.MaxTextureSlots; i++)
			samplers[i] = i;

		s_RenderData.QuadShader = Shader::Create("../../Data/Shaders/Texture.glsl");
		s_RenderData.CircleShader = Shader::Create("../../Data/Shaders/Circle.glsl");
		s_RenderData.LineShader = Shader::Create("../../Data/Shaders/Line.glsl");

		// Set first texture slot to 0
		s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;

		s_RenderData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_RenderData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_RenderData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_RenderData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_RenderData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		SAZ_PROFILE_FUNCTION();

		delete[] s_RenderData.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_RenderData.CameraUniformBuffer->SetData(&s_RenderData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		SAZ_PROFILE_FUNCTION();

		s_RenderData.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_RenderData.CameraUniformBuffer->SetData(&s_RenderData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		SAZ_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_RenderData.QuadIndexCount = 0;
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;

		s_RenderData.CircleIndexCount = 0;
		s_RenderData.CircleVertexBufferPtr = s_RenderData.CircleVertexBufferBase;

		s_RenderData.LineVertexCount = 0;
		s_RenderData.LineVertexBufferPtr = s_RenderData.LineVertexBufferBase;

		s_RenderData.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_RenderData.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase);
			s_RenderData.QuadVertexBuffer->SetData(s_RenderData.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_RenderData.TextureSlotIndex; i++)
				s_RenderData.TextureSlots[i]->Bind(i);

			s_RenderData.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray, s_RenderData.QuadIndexCount);
			s_RenderData.Stats.DrawCalls++;
		}

		if (s_RenderData.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.CircleVertexBufferPtr - (uint8_t*)s_RenderData.CircleVertexBufferBase);
			s_RenderData.CircleVertexBuffer->SetData(s_RenderData.CircleVertexBufferBase, dataSize);

			s_RenderData.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_RenderData.CircleVertexArray, s_RenderData.CircleIndexCount);
			s_RenderData.Stats.DrawCalls++;
		}

		if (s_RenderData.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.LineVertexBufferPtr - (uint8_t*)s_RenderData.LineVertexBufferBase);
			s_RenderData.LineVertexBuffer->SetData(s_RenderData.LineVertexBufferBase, dataSize);

			s_RenderData.LineShader->Bind();
			RenderCommand::SetLineWidth(s_RenderData.LineWidth);
			RenderCommand::DrawLines(s_RenderData.LineVertexArray, s_RenderData.LineVertexCount);
			s_RenderData.Stats.DrawCalls++;
		}
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		SAZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SAZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		SAZ_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_RenderData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.QuadVertexBufferPtr->Color = color;
			s_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_RenderData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RenderData.QuadVertexBufferPtr->EntityID = entityID;
			s_RenderData.QuadVertexBufferPtr++;
		}

		s_RenderData.QuadIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		SAZ_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_RenderData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_RenderData.TextureSlotIndex; i++)
		{
			if (*s_RenderData.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_RenderData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = texture;
			s_RenderData.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.QuadVertexBufferPtr->Color = tintColor;
			s_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_RenderData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RenderData.QuadVertexBufferPtr->EntityID = entityID;
			s_RenderData.QuadVertexBufferPtr++;
		}

		s_RenderData.QuadIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		SAZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		SAZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		SAZ_PROFILE_FUNCTION();

		// TODO: implement for circles
		// if (s_RenderData.QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_RenderData.CircleVertexBufferPtr->WorldPosition = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.CircleVertexBufferPtr->LocalPosition = s_RenderData.QuadVertexPositions[i] * 2.0f;
			s_RenderData.CircleVertexBufferPtr->Color = color;
			s_RenderData.CircleVertexBufferPtr->Thickness = thickness;
			s_RenderData.CircleVertexBufferPtr->Fade = fade;
			s_RenderData.CircleVertexBufferPtr->EntityID = entityID;
			s_RenderData.CircleVertexBufferPtr++;
		}

		s_RenderData.CircleIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, component::SpriteComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_RenderData.LineVertexBufferPtr->Position = p0;
		s_RenderData.LineVertexBufferPtr->Color = color;
		s_RenderData.LineVertexBufferPtr->EntityID = entityID;
		s_RenderData.LineVertexBufferPtr++;

		s_RenderData.LineVertexBufferPtr->Position = p1;
		s_RenderData.LineVertexBufferPtr->Color = color;
		s_RenderData.LineVertexBufferPtr->EntityID = entityID;
		s_RenderData.LineVertexBufferPtr++;

		s_RenderData.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_RenderData.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_RenderData.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_RenderData.Stats;
	}

}
