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
		virtual void Init();
		virtual void PostInit();
		virtual void Destroy();

		virtual void Update(const Saz::GameTime& gameTime);
		void NewScene();
		void LoadScene(const String& scenePath);
		void SaveScene(const String& scenePath);
		void UpdateWindowName(const String& scenePath);
	};
}
