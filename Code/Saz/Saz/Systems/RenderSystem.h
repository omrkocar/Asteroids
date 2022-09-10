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
		virtual void ImGuiRender() override;

		bool IsViewportFocused() { return m_ViewportFocused; }

	private:
		Saz::WindowBase& m_Window;
		CameraSystem& m_CameraSystem;

		glm::vec2 m_SceneSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewPortHovered = false;
		Saz::Ref<Saz::FrameBuffer> m_FrameBuffer = nullptr;
		Entity m_SquareEntity = entt::null;
	};
}
