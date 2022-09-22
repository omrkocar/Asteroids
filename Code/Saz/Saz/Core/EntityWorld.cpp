#include "SazPCH.h"
#include "EntityWorld.h"
#include "Saz/Systems/System.h"

#include "Saz/Components/CameraComponent.h"
#include "Saz/Components/InputComponent.h"
#include "Saz/Components/NameComponent.h"
#include "Saz/Components/PhysicsComponents.h"
#include "Saz/Components/RenderComponents.h"
#include "Saz/Components/SceneComponent.h"
#include "Saz/Components/TransformComponent.h"
#include "Saz/Core/UUID.h"

namespace ecs
{
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

	ecs::Entity EntityWorld::CreateBaseEntity(const String& name)
	{
		return CreateBaseEntity(Saz::UUID(), name);
	}

	Entity EntityWorld::CreateBaseEntity(Saz::UUID uuid, const String& name)
	{
		auto entity = CreateEntity();
		auto& nameComp = AddComponent<component::NameComponent>(entity);
		nameComp.Name = name;
		AddComponent<component::TransformComponent>(entity);
		AddComponent<component::SceneEntityComponent>(entity);
		auto& idComp = AddComponent<component::IDComponent>(entity);
		idComp.ID = uuid;
		m_EntityIDMap[uuid] = entity;
		SortEntities();
		return entity;
	}

	void EntityWorld::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateBaseEntity();
		CopyComponentIfExists<component::SpriteComponent>(newEntity, entity);
		CopyComponentIfExists<component::Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<component::BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<component::CircleCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<component::NameComponent>(newEntity, entity);
		CopyComponentIfExists<component::CameraComponent>(newEntity, entity);
		CopyComponentIfExists<component::CircleRendererComponent>(newEntity, entity);
		CopyComponentIfExists<component::SceneEntityComponent>(newEntity, entity);
		CopyComponentIfExists<component::TransformComponent>(newEntity, entity);
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

	void EntityWorld::SortEntities()
	{
		m_Registry.sort<component::IDComponent>([&](const auto lhs, const auto rhs)
			{
				auto lhsEntity = m_EntityIDMap.find(lhs.ID);
				auto rhsEntity = m_EntityIDMap.find(rhs.ID);
				return static_cast<uint32_t>(lhsEntity->second) < static_cast<uint32_t>(rhsEntity->second);
			});
	}

	ecs::Entity EntityWorld::CreateMainCamera()
	{
		auto mainCamera = CreateEntity();
		auto& cameraComp = AddComponent<component::EditorCameraComponent>(mainCamera);
		cameraComp.Camera.Setup(30.0f, 1.778f, 0.1f, 1000.0f, 0.0f, 0.0f, 50.0f);
		AddComponent<component::InputComponent>(mainCamera);
		AddComponent<component::TransformComponent>(mainCamera);
		AddComponent<component::NameComponent>(mainCamera, "Editor Camera");
		
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

