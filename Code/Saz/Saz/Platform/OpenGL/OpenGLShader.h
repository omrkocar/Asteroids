#pragma once

#include "Saz/Rendering/Shader.h"
#include "glm/glm.hpp"

namespace Saz
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexSource, const char* fragmentSource);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const String& name, int value);

		void UploadUniformFloat(const String& name, float value);
		void UploadUniformFloat2(const String& name, const glm::vec2& values);
		void UploadUniformFloat3(const String& name, const glm::vec3& values);
		void UploadUniformFloat4(const String& name, const glm::vec4& values);

		void UploadUniformMat3(const String& name, const glm::mat3& matrix);
		void UploadUniformMat4(const String& name, const glm::mat4& matrix);

	private:
		void Compile(const char* vertexSource, const char* fragmentSource);

	private:
		uint32_t m_RendererID;
	};
}