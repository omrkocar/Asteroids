#pragma once

#include <Saz/System.h>

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
	class SceneEditor final : public System
	{
	public:
		
		SceneEditor(Saz::sfml::Window& sfmlWindow);

		virtual void Init() override;
		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		void Render();

	private:
		Saz::sfml::Window& m_SFMLWindow;
		bool m_IsVisible = true;
	};
}
