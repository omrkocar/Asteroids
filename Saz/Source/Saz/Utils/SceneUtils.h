#pragma once

#include <Core/String.h>
#include <Saz/Core/Entity.h>
#include "Saz/Core/EntityWorld.h"
#include <filesystem>

namespace Saz
{
	class SceneUtils
	{
	public:
		static void OpenScene(ecs::EntityWorld& world, ecs::Entity entity);
		static void OpenScene(ecs::EntityWorld& world, ecs::Entity entity, const std::filesystem::path& path);
	};
}