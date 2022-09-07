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

		virtual void Update(const Saz::GameTime& gameTime) override;

		void DrawWorldOutliner();
		bool m_IsActive = true;
		bool m_IsObjectInspectorOn = true;
		ecs::Entity m_SelectedEntity = entt::null;
	};
}
