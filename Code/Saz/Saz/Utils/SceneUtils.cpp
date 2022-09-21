#include "SazPCH.h"
#include "SceneUtils.h"
#include "PlatformUtils.h"
#include "Saz/SceneComponent.h"

namespace Saz
{
	void SceneUtils::OpenScene(ecs::EntityWorld& world, ecs::Entity entity)
	{
		const String& path = Saz::FileDialogs::OpenFile("Saz Scene (*.saz)\0*.saz\0");
		if (!path.empty())
		{
			auto& sceneComponent = world.AddComponent<component::LoadSceneRequestOneFrameComponent>(entity);
			sceneComponent.Path = path;
		}
	}

	void SceneUtils::OpenScene(ecs::EntityWorld& world, ecs::Entity entity, const std::filesystem::path& path)
	{
		if (path.extension().string() != ".saz")
			return;

		auto& sceneComponent = world.AddComponent<component::LoadSceneRequestOneFrameComponent>(entity);
		sceneComponent.Path = path.string();
	}
}