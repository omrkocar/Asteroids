#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
	class Camera;
}

namespace ecs 
{
	class RenderSystem;

	class CameraSystem final : public System
	{
	public:
		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

	private:
		void OnWindowResized(Entity entity);
	};
}
