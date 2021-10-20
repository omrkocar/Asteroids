#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	// use/activate the shader
	void Use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void checkCompileErrors(GLuint shader, std::string type);
};
