#include "SazPCH.h"
#include "EntityWorld.h"

#include "Saz/Systems/System.h"
#include "Saz/NameComponent.h"
#include "CameraComponent.h"
#include "InputComponent.h"
#include "TransformComponent.h"
#include "SceneComponent.h"
#include "PhysicsComponents.h"
#include "RenderComponents.h"
#include "UUID.h"

namespace ecs
{
	template<typename T>
	static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<Saz::UUID, Entity>& enttMap)
	{
		auto components = srcRegistry.view<T>();
		for (auto srcEntity : components)
		{
			entt::entity destEntity = enttMap.at(srcRegistry.get<component::IDComponent>(srcEntity).ID);

			auto& srcComponent = srcRegistry.get<T>(srcEntity);
			if (!srcRegistry.valid(destEntity))
			{
				int bp = 1;
			}
			auto& destComponent = dstRegistry.emplace_or_replace<T>(destEntity, srcComponent);
		}
	}

	void EntityWorld::Init()
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->Init();
	}

	void EntityWorld::Destroy()
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
		{
			entry.m_System->Destroy();
			delete entry.m_System;
		}

		m_SystemEntries.clear();
		m_ComponentEntries.clear();
	}

	void EntityWorld::Update(const Saz::GameTime& gameTime)
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->Update(gameTime);
	}

	void EntityWorld::CopyEntities()
	{
	}

	ecs::Entity EntityWorld::CreateBaseEntity()
	{
		return CreateBaseEntity(Saz::UUID(), "Empty Object");
	}

	Entity EntityWorld::CreateBaseEntity(Saz::UUID uuid, const String& name)
	{
		auto entity = CreateEntity();
		AddComponent<component::NameComponent>(entity, name);
		AddComponent<component::TransformComponent>(entity);
		AddComponent<component::SceneEntityComponent>(entity);
		auto& idComp = AddComponent<component::IDComponent>(entity);
		idComp.ID = uuid;
		return entity;
	}

	const Saz::UUID& EntityWorld::GetUUID(Entity entity)
	{
		return GetComponent<component::IDComponent>(entity).ID;
	}

	ecs::Entity EntityWorld::TryGetEntityWithTag(const String& tag)
	{
		auto entities = GetAllEntitiesWith<component::NameComponent>();
		for (auto e : entities)
		{
			if (entities.get<component::NameComponent>(e).Name == tag)
				return e;
		}

		return entt::null;
	}

	void EntityWorld::DestroyAllEntities()
	{
		m_Registry.each([this](auto entity) {
			m_Registry.destroy(entity);
		});
	}

	ecs::Entity EntityWorld::CreateMainCamera()
	{
		auto mainCamera = CreateBaseEntity(Saz::UUID(), "Main Camera");
		auto& cameraComp = AddComponent<component::EditorCameraComponent>(mainCamera);
		cameraComp.Camera.Setup(30.0f, 1.778f, 0.1f, 1000.0f, 0.01f, 0.01f);
		AddComponent<component::InputComponent>(mainCamera);
		
		SetMainCamera(mainCamera);
		return mainCamera;
	}

	ecs::Entity EntityWorld::GetMainCameraEntity()
	{
		return m_MainCamera;
	}

	void EntityWorld::SetMainCamera(Entity entity)
	{
		m_MainCamera = entity;
	}

}

