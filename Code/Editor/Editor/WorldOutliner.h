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
		virtual void ImGuiRender() override;

		bool IsVisible() { return m_IsActive; }
		void SetVisible(bool active) { m_IsActive = active; }

		void DrawWorldOutliner();
		bool m_IsActive = true;
		bool m_IsObjectInspectorOn = true;
		ecs::Entity m_SelectedEntity = entt::null;
		Entity m_EntityToDelete = entt::null;

	private:
		void DrawEntityNode(Entity entity);

		void OnSceneLoadRequest(entt::registry& registry, entt::entity entity);
	};
}
