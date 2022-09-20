#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
	class Texture2D;
}

namespace ecs 
{
	class Toolbar final : public System
	{
	public:
		
		Toolbar();

		virtual void Init() override;

		virtual void Update(const Saz::GameTime& gameTime) override;

	private:
		void ImGuiRender();
		void DrawToolbar();

		void OnScenePlay();
		void OnSceneStop();
	private:
		Saz::Ref<Saz::Texture2D> m_PlayIcon;
		Saz::Ref<Saz::Texture2D> m_StopIcon;

		Entity m_Entity;
	};
}
