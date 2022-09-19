#pragma once

#include <Saz/Systems/System.h>
#include <Editor/WorldOutliner.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class Inspector final : public System
	{
	public:
		
		Inspector(WorldOutliner& worldOutliner);

		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		void ImGuiRender();

		void DrawComponents(Entity entity);

		void DrawAddComponentPopup(Entity entity);

		void DrawNameComponent(Entity entity);
		void DrawTransformComponent(Entity entity);
		void DrawCameraComponent(Entity entity);
		void DrawSpriteComponent(Entity entity);
		void DrawRigidbody2DComponent(Entity entity);
		void DrawBoxCollider2DComponent(Entity entity);

	private:
		WorldOutliner& m_WorldOutliner;
	};
}
