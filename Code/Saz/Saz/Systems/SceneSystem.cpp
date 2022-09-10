#include "SazPCH.h"
#include "SceneSystem.h"

#include "Saz/CameraComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/TransformComponent.h"

#include <entt/entt.hpp>
#include "Saz/Core/Application.h"
#include "SceneComponent.h"

namespace ecs
{	
	SceneSystem::SceneSystem()
	{

	}

	void SceneSystem::Init()
	{
		Entity sceneEntity = m_World->CreateEntity();
		auto& sceneComp = m_World->AddComponent<component::SceneComponent>(sceneEntity);
	}

	void SceneSystem::PostInit()
	{

	}

	void SceneSystem::Destroy()
	{

	}

	void SceneSystem::Update(const Saz::GameTime& gameTime)
	{

	}
}
