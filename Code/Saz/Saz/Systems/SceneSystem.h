#pragma once

#include <Saz/Systems/System.h>

namespace Saz
{
	class GameTime;
}

namespace ecs 
{
	class SceneSystem final : public System
	{
	public:
		
		SceneSystem();

		virtual void Init();
		virtual void PostInit();
		virtual void Destroy();

		virtual void Update(const Saz::GameTime& gameTime);
		void NewScene(Entity newSceneEntity);
		void LoadScene(Entity loadSceneEntity, const String& scenePath);
		void SaveScene(Entity saveSceneEntity, const String& scenePath);
		void UpdateWindowName(const String& scenePath);

	private:
		Entity m_SceneEntity;
	};
}
