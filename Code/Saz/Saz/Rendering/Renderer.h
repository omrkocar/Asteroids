#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Saz/Rendering/OrtographicCamera.h"
#include <memory>


namespace Saz
{
	class Shader;

	class Renderer
	{
	public:
		static void BeginScene(OrtographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}
