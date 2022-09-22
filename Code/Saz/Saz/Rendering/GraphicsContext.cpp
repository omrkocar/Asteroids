#include "SazPCH.h"
#include "Saz/Rendering/GraphicsContext.h"

#include "Saz/Rendering/Renderer.h"
#include "Saz/Platform/OpenGL/OpenGLContext.h"

namespace Saz {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}