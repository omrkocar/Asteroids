#pragma once

#include <Saz/Systems/System.h>
#include "glm/glm.hpp"

namespace Saz
{
	class GameTime;
	class FrameBuffer;
}

namespace ecs 
{
	class SceneEditor final : public System
	{
	public:
		
		SceneEditor();

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		virtual void ImGuiRender() override;


		bool IsViewportFocused() const { return m_ViewportFocused; }

	private:
		void OnCameraComponentAdded(entt::registry& registry, entt::entity entity);
		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();

		void DrawScene();
		void DrawProfiler();
		void DrawMenuBar();

	private:
		bool m_ViewportFocused = false;
		bool m_ViewPortHovered = false;
		glm::vec2 m_SceneSize = { 0.0f, 0.0f };
		Saz::Ref<Saz::FrameBuffer> m_FrameBuffer = nullptr;
	};
}
