#pragma once

#include <Saz/Systems/System.h>
#include "Core/WindowBase.h"
#include "OrthographicCameraController.h"
#include "glm/glm.hpp"

namespace Saz { class FrameBuffer; }

namespace Saz
{
	class GameTime;

	class WindowBase;
}
namespace ecs 
{
	class RenderSystem final : public System
	{
	public:
		
		RenderSystem(Saz::WindowBase& window);
		~RenderSystem();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		virtual void ImGuiRender() override;

	private:
		Saz::WindowBase& m_Window;
		Saz::OrthographicCameraController m_CameraController;


		glm::vec2 m_SceneSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewPortHovered = false;
		Saz::Ref<Saz::FrameBuffer> m_FrameBuffer;
		Entity m_SquareEntity;
	};
}
