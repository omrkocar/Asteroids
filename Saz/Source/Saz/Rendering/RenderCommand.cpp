#include "SazPCH.h"
#include "RenderCommand.h"

#include "Saz/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Saz
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}