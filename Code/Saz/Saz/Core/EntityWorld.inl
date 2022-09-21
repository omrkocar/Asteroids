#pragma once

#include <Saz/TypeId.h>
#include <Saz/Core/Log.h>
#include <Saz/NameComponent.h>

inline bool ecs::EntityWorld::IsAlive(const ecs::Entity& entity) const
{
	return m_Registry.valid(entity);
}

inline auto ecs::EntityWorld::CreateEntity()->ecs::Entity
{
	return m_Registry.create();
}

template<typename Component>
void ecs::EntityWorld::CopyComponentIfExists(ecs::Entity dst, ecs::Entity src)
{
	if (m_Registry.has<Component>(src))
	{
		auto& srcComponent = m_Registry.get<Component>(src);
		m_Registry.emplace_or_replace<Component>(dst, srcComponent);
	}
}

inline void ecs::EntityWorld::DestroyEntity(const ecs::Entity& entity)
{
	if (HasComponent<component::IDComponent>(entity))
	{
		Saz::UUID id = GetUUID(entity);
		m_EntityIDMap.erase(id);
	}

	m_Registry.destroy(entity);
}

template<class TComponent>
bool ecs::EntityWorld::HasComponent(const ecs::Entity& entity) const
{
	return m_Registry.has<TComponent>(entity);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::GetComponent(const ecs::Entity& entity)->TComponent&
{
	SAZ_CORE_ASSERT(HasComponent<TComponent>(entity), "Entity doesn't have this component!");
	return m_Registry.get<TComponent>(entity);
}

template<class TComponent>
auto ecs::EntityWorld::GetSingleComponent()->TComponent&
{
	for (const ecs::Entity& entity : m_Registry.view<TComponent>())
		return GetComponent<TComponent>(entity);

	SAZ_CORE_ASSERT(false, "Trying to fetch a component that doesn't exist!");
	return GetComponent<TComponent>(entt::null);
}

template<typename... Components>
auto ecs::EntityWorld::GetAllEntitiesWith()
{
	return m_Registry.view<Components...>();
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::TryGetComponent(const ecs::Entity& entity)->TComponent*
{
	if (HasComponent(entity))
	{
		return GetComponent(entity);
	}

	return nullptr;
}

template<class TComponent, typename... TArgs>
void ecs::EntityWorld::DestroyEntitesWith()
{
	auto view = GetAllEntitiesWith<TComponent>();
	for (auto& entity : view)
	{
		m_Registry.destroy(entity);
	}
}

template<class TComponent>
void ecs::EntityWorld::RegisterComponent()
{
	static_assert(std::is_assignable<TComponent, TComponent>::value, "Component isn't assignable!");

	ComponentEntry entry =
	{
		Saz::ToTypeId<TComponent>()
	};

	
	m_ComponentEntries.push_back(entry);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddComponent(const ecs::Entity& entity, TArgs&&... args)->TComponent&
{
	SAZ_CORE_ASSERT(!m_Registry.has<TComponent>(entity), "Attempting to add a component that already exists on the Entity. Investigate!");
	return m_Registry.emplace<TComponent>(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityWorld::RemoveComponent(const ecs::Entity& entity)
{
	SAZ_CORE_ASSERT(m_Registry.has<TComponent>(entity), "Attempting to remove a component that does not exist on the Entity!");
	m_Registry.remove<TComponent>(entity);
}

template<class TSystem>
TSystem& ecs::EntityWorld::GetSystem()
{
	constexpr Saz::TypeId typeId = Saz::ToTypeId<TSystem>();

	auto result = std::find_if(m_SystemEntries.begin(), m_SystemEntries.end(),
		[&typeId](const SystemEntry& entry)
		{
			return entry.m_TypeId == typeId;
		});

	SAZ_CORE_ASSERT((result != m_SystemEntries.end()), "Attempting to access an invalid system or it has not been registered yet!");
	return *dynamic_cast<TSystem*>(result->m_System);
}

template<class TSystem, typename... TArgs>
void ecs::EntityWorld::RegisterSystem(TArgs&&... args)
{
	static_assert(std::is_convertible<TSystem*, ecs::System*>::value, "System must inherit from ecs::System using the [public] keyword!");
	static_assert(std::is_base_of<ecs::System, TSystem>::value, "System isn't a base of ecs::System!");

	SystemEntry entry =
	{
		Saz::ToTypeId<TSystem>()
		, new TSystem(std::forward<TArgs>(args)...)
	};
	entry.m_System->m_World = this;

	m_SystemEntries.push_back(entry);
}