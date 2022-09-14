#include "SazPCH.h"
#include "RenderSystem.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/TransformComponent.h"

#include "Saz/Core/WindowBase.h"

#include "Rendering/Renderer2D.h"
#include "Rendering/RenderCommand.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Rendering/Renderer.h"

namespace ecs
{
	RenderSystem::RenderSystem(Saz::WindowBase& window, CameraSystem& cameraSystem)
		: m_Window(window)
		, m_CameraSystem(cameraSystem)
	{

	}

	RenderSystem::~RenderSystem()
	{

	}

	void RenderSystem::Init()
	{

	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

	}
}
