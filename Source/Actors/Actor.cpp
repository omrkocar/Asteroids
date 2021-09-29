#include "GamePCH.h"
#include "Actor.h"
#include "Core/World.h"
#include "Managers/ComponentManager.h"
#include "Components/Component.h"

Actor::Actor(World* pWorld, const char* name)
{
	m_pWorld = pWorld;
	m_Name = name;
}

Actor::~Actor()
{
	for (Component* pComp : m_Components)
	{
		m_pWorld->GetComponentManager()->RemoveComponent(pComp);
		delete pComp;
	}
}

void Actor::AddComponent(Component* pComponent)
{
	pComponent->SetOwner(this);
	pComponent->Init();
	m_pWorld->GetComponentManager()->AddComponent(pComponent);
	m_Components.push_back(pComponent);
}

Component* Actor::GetFirstComponentOfType(const char* type)
{
	for (Component* pComponent : m_Components)
	{
		if (pComponent->GetType() == type)
		{
			return pComponent;
		}
	}

	return nullptr;
}
