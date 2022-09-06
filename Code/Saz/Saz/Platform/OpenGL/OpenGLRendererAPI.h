#pragma once

#include "Saz/Rendering/RendererAPI.h"

namespace Saz
{
	class OpenGLRendererAPI : public RendererAPI
	{

	public:
		void Init() override;
		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;
		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};
}