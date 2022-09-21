#pragma once

#include <Saz/Systems/System.h>

namespace component 
{ 
	struct LoadedSceneComponent; 
}

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
		virtual void Destroy();

		virtual void Update(const Saz::GameTime& gameTime);

	private:
		void NewScene();
		void LoadScene(const String& scenePath);
		void SaveScene(const String& scenePath);
		void UpdateWindowName(const String& scenePath);
		void DestroySceneEntities();

		component::LoadedSceneComponent* m_Scene;

	private:
		void OnSceneStateChangeRequest(entt::registry& registry, entt::entity entity);
	};
}
