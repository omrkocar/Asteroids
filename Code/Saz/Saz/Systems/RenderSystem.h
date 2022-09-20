#pragma once

#include <Saz/Systems/System.h>
#include <Saz/Core/WindowBase.h>
#include <glm/glm.hpp>

namespace ecs
{
	class CameraSystem;
}

namespace Saz
{
	class GameTime;
	class FrameBuffer;

	class WindowBase;
}
namespace ecs 
{
	class RenderSystem final : public System
	{
	public:
		
		RenderSystem(Saz::WindowBase& window, CameraSystem& cameraSystem);
		~RenderSystem();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		void RenderScene();
		void RenderRuntime();
	private:
		Saz::WindowBase& m_Window;
		CameraSystem& m_CameraSystem;
	};
}
