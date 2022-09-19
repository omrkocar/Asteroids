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
		virtual void Update(const Saz::GameTime& gameTime) override;
		virtual void ImGuiRender();

		Entity m_CameraEntity;
		Entity m_SecondCamera;

	private:
		void OnWindowResized(entt::registry& registry, entt::entity entity);
	};
}
