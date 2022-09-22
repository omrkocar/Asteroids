#include "SazPCH.h"
#include "FrameBuffer.h"

#include "Saz/Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Saz/Rendering/Renderer.h"

namespace Saz
{
	
	Saz::Ref<Saz::FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
				return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameBuffer>(spec);
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}