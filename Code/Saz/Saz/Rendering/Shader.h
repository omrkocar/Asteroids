#pragma once

#include "Core/String.h"

namespace Saz
{
	class Shader
	{
	public:
		Shader(const String& vertexSource, const String& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};
}