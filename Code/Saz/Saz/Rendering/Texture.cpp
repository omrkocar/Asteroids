#include "SazPCH.h"
#include "Texture.h"
#include "Saz/Rendering/RendererAPI.h"
#include "Saz/Platform/OpenGL/OpenGLTexture.h"
#include "Saz/Rendering/Renderer.h"

namespace Saz
{
	Ref<Saz::Texture2D> Texture2D::Create(const String& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SAZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
				return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		SAZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}