#pragma once

#include "Core/String.h"

#include <glm/glm.hpp>

namespace Saz
{
	class Shader
	{
	public:
		Shader(const String& vertexSource, const String& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const String& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};
}