#include "SazPCH.h"
#include "UniformBuffer.h"

#include "Saz/Rendering/Renderer.h"
#include "Saz/Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Saz {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}