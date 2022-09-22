#include "SazPCH.h"
#include "VertexArray.h"

#include "Saz/Rendering/Renderer.h"
#include "Saz/Platform/OpenGL/OpenGLVertexArray.h"

namespace Saz
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
				return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}