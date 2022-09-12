#include "SazPCH.h"
#include "EntityWorld.h"

#include "Saz/Systems/System.h"
#include "Saz/NameComponent.h"
#include "CameraComponent.h"

namespace ecs
{
	void EntityWorld::Init()
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->Init();
	}

	void EntityWorld::PostInit()
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->PostInit();
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

	void EntityWorld::PreUpdate(const Saz::GameTime& gameTime)
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->PreUpdate(gameTime);
	}

	void EntityWorld::Update(const Saz::GameTime& gameTime)
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->Update(gameTime);
	}

	void EntityWorld::LateUpdate(const Saz::GameTime& gameTime)
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->LateUpdate(gameTime);
	}

	void EntityWorld::ImGuiRender()
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->ImGuiRender();
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

	ecs::Entity EntityWorld::GetMainCameraEntity()
	{
		return m_MainCamera;
	}

	void EntityWorld::SetMainCamera(Entity entity)
	{
		m_MainCamera = entity;
	}

}

