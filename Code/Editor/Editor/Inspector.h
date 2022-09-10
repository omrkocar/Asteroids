#pragma once

#include <Saz/Systems/System.h>
#include <Editor/WorldOutliner.h>

namespace Saz
{
	class GameTime;

	namespace sfml
	{
		class Window;
	}
}

namespace ecs 
{
	class Inspector final : public System
	{
	public:
		
		Inspector(WorldOutliner& worldOutliner);

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;
		virtual void ImGuiRender() override;

		void DrawComponents(Entity entity);

	private:
		WorldOutliner& m_WorldOutliner;
	};
}
