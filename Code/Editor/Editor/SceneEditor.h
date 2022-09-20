#pragma once

#include <Saz/Systems/System.h>
#include "glm/glm.hpp"
#include "WorldOutliner.h"

namespace Saz { class Texture2D; }

namespace component { struct LoadedSceneComponent; }

namespace ImGuizmo
{
	enum OPERATION;
}

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
		
		SceneEditor(WorldOutliner& worldOutliner);

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

		void ProcessMousePicking(Saz::Ref<Saz::FrameBuffer> frameBuffer);

		void RenderScene();
		void RenderRuntime();
		void RenderOverlay();

		void ProcessInput();

		void ImGuiRender();

		bool IsViewportFocused() const { return m_ViewportFocused; }
		bool IsViewportHovered() const { return m_ViewPortHovered; }
 
	private:
		void OnLevelLoaded(entt::registry& registry, entt::entity entity);

		void DrawScene();

		void DrawGizmos();

		void DrawProfiler();
		void DrawMenuBar();
		void DrawToolbar();

		void OnScenePlay();
		void OnSceneStop();

		Entity m_HoveredEntity = entt::null;

	private:
		WorldOutliner& m_WorldOutliner;
		component::LoadedSceneComponent* m_Scene;

		Saz::Ref<Saz::Texture2D> m_PlayIcon;
		Saz::Ref<Saz::Texture2D> m_StopIcon;

		bool m_ViewportFocused = false;
		bool m_ViewPortHovered = false;
		glm::vec2 m_SceneSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		int m_GizmoType = 0;
		bool m_IsGizmoVisible = true;
		Saz::Ref<Saz::FrameBuffer> m_FrameBuffer = nullptr;
		Entity m_Entity = entt::null;
	};
}
