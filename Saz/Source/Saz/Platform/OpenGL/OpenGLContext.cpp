#include "SazPCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Saz::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	SAZ_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
}

void Saz::OpenGLContext::Init()
{
	SAZ_PROFILE_FUNCTION();

	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	SAZ_CORE_ASSERT(status, "Failed to initialize Glad!");

	SAZ_CORE_INFO("OpenGL Info:");
	SAZ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
	SAZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
	SAZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
}

void Saz::OpenGLContext::SwapBuffers()
{
	SAZ_PROFILE_FUNCTION();

	glfwSwapBuffers(m_WindowHandle);
}
