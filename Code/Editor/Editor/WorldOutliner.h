#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class WorldOutliner final : public System
	{
	public:
		
		WorldOutliner();

		virtual void Init() override;

		virtual void Update(const Saz::GameTime& gameTime) override;
		void ImGuiRender();

		bool IsVisible() { return m_IsActive; }
		void SetVisible(bool active) { m_IsActive = active; }

		bool m_IsActive = true;
		bool m_IsObjectInspectorOn = true;
		ecs::Entity m_SelectedEntity = entt::null;
		Entity m_EntityToDelete = entt::null;

	private:
		void DrawEntityNode(Entity entity);

		void OnSceneLoadRequest(entt::registry& registry, entt::entity entity);
	};
}
