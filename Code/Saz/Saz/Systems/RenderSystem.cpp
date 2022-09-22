#include "SazPCH.h"
#include "RenderSystem.h"

#include "Saz/Components/CameraComponent.h"
#include "Saz/Components/RenderComponents.h"
#include "Saz/Components/TransformComponent.h"
#include "Saz/Core/WindowBase.h"
#include "Saz/Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Renderer.h"
#include "Saz/Rendering/Renderer2D.h"
#include "Saz/Systems/CameraSystem.h"


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
		
	}

	void RenderSystem::RenderScene()
	{
		
	}

	void RenderSystem::RenderRuntime()
	{
		
	}

}
