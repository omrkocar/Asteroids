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

		static Ref<Shader> Create(const String& vertexSource, const String& fragmentSource);
	};
}