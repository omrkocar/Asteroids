#include "SazPCH.h"
#include "EntityWorld.h"

#include "Saz/System.h"
#include "TransformComponent.h"

namespace ecs
{
	void EntityWorld::Init()
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->Init();
	}

	void EntityWorld::Destroy()
	{
		m_ComponentEntries.clear();
		for (ecs::SystemEntry& entry : m_SystemEntries)
		{
			entry.m_System->Destroy();
			delete entry.m_System;
		}
		m_SystemEntries.clear();
	}

	void EntityWorld::Update(const Saz::GameTime& gameTime)
	{
		for (ecs::SystemEntry& entry : m_SystemEntries)
			entry.m_System->Update(gameTime);
	}

	void EntityWorld::DestroyAllEntities()
	{
		m_Registry.each([this](auto entity) {
			m_Registry.destroy(entity);
			});
	}

}

