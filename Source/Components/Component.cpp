#include "GamePCH.h"
#include "Component.h"
#include "Actors/Actor.h"

Component::Component()
{

}

Component::~Component()
{

}

void Component::SetOwner(Actor* pOwner)
{
	m_pOwner = pOwner;
}

Actor* Component::GetOwner()
{
	return m_pOwner;
}

void Component::Activate()
{
	m_IsActive = true;
}

void Component::Deactivate()
{
	m_IsActive = false;
}

void Component::ToggleActive()
{
	m_IsActive = !m_IsActive;
}

const bool Component::IsActive() const
{
	return m_IsActive;
}

