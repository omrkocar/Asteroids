#include "SazPCH.h"
#include "Shader.h"

#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "Saz/Rendering/Renderer.h"

namespace Saz
{
	Ref<Shader> Shader::Create(const String& filename)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
				return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filename);
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	Ref<Saz::Shader> Shader::Create(const String& name, const String& vertexSource, const String& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
				return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);;
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	// Shader Library
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const String& name, const Ref<Shader>& shader)
	{
		SAZ_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Saz::Ref<Saz::Shader> ShaderLibrary::Load(const String& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Saz::Ref<Saz::Shader> ShaderLibrary::Load(const String& name, const String& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const String& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	Saz::Ref<Saz::Shader> ShaderLibrary::Get(const String& name)
	{
		SAZ_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

}