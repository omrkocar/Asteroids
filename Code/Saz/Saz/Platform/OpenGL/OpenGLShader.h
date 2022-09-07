#pragma once

#include "Saz/Rendering/Shader.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <fstream>
#include <glad/glad.h>

namespace Saz
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const String& filepath);
		OpenGLShader(const String& name, const String& vertexSrc, const String& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const String& name, const glm::mat4& value) override;
		virtual void SetFloat3(const String& name, const glm::vec3& value) override;
		virtual void SetFloat4(const String& name, const glm::vec4& value) override;
		virtual void SetInt(const String& name, const int value) override;

		virtual const String& GetName() const override { return m_Name; }

		void UploadUniformInt(const String& name, int value);

		void UploadUniformFloat(const String& name, float value);
		void UploadUniformFloat2(const String& name, const glm::vec2& values);
		void UploadUniformFloat3(const String& name, const glm::vec3& values);
		void UploadUniformFloat4(const String& name, const glm::vec4& values);

		void UploadUniformMat3(const String& name, const glm::mat3& matrix);
		void UploadUniformMat4(const String& name, const glm::mat4& matrix);

	private:
		String ReadFile(const String& filepath);
		std::unordered_map<GLenum, String> PreProcess(const String& source);
		void Compile(const std::unordered_map<GLenum, String>& shaderSources);

	private:
		uint32_t m_RendererID;
		String m_Name;
	};
}