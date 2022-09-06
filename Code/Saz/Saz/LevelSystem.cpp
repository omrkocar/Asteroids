#include "SazPCH.h"
#include "LevelSystem.h"

#include "Saz/FileHelpers.h"
#include "Saz/InputComponent.h"
#include "Saz/CameraComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/ResourceManager.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>
#include "Saz/Application.h"
#include "Saz/ResourceManager.h"

namespace ecs
{	
	LevelSystem::LevelSystem(Saz::ResourceManager& resourceManager)
		: m_ResourceManager(resourceManager)
	{

	}

	bool LevelSystem::LoadFromFile(const String& filename)
	{
		m_World->DestroyAllEntities();

		return true;
	}

}
