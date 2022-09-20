#pragma once

#include <Saz/Systems/System.h>
#include "glm/ext/vector_float2.hpp"

namespace Saz
{
	class GameTime;
	class FrameBuffer;
}

namespace ecs 
{
	class GameViewport final : public System
	{
	public:
		
		GameViewport();

		virtual void Init() override;

		virtual void Update(const Saz::GameTime& gameTime) override;
		void ImGuiRender();
		void RenderRuntime();
		void DrawViewport();
	private:
		Entity m_FrameBufferEntity;
		Entity m_Entity;

		glm::vec2 m_ViewportSize{ 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewPortHovered = false;
		glm::vec2 m_ViewportBounds[2];

		Saz::Ref<Saz::FrameBuffer> m_FrameBuffer = nullptr;
	};
}
