#pragma once

#include "Saz/Rendering/GraphicsContext.h"

struct GLFWwindow;

namespace Saz
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}