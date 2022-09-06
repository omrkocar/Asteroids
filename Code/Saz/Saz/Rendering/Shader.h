#pragma once

#include "Core/String.h"
#include "Saz/Core.h"

namespace Saz
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const String& GetName() const = 0;

		static Ref<Shader> Create(const String& filename);
		static Ref<Shader> Create(const String& name, const String& vertexSource, const String& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const String& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const String& filepath);
		Ref<Shader> Load(const String& name, const String& filepath);

		bool Exists(const String& name) const;

		Ref<Shader> Get(const String& name);
	private:
		std::unordered_map<String, Ref<Shader>> m_Shaders;
	};
}