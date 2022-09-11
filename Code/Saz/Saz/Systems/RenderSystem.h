#pragma once

#include <Saz/Systems/System.h>
#include "Core/WindowBase.h"
#include "glm/glm.hpp"
#include "CameraSystem.h"

namespace ecs
{

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

	private:
		Saz::WindowBase& m_Window;
		CameraSystem& m_CameraSystem;

		Entity m_SquareEntity = entt::null;
	};
}
