#pragma once

#include "Core/String.h"

namespace Saz
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Shader> Create(const char* vertexSource, const char* fragmentSource);
	};
}