#include "GamePCH.h"
#include "World.h"
#include "Managers/ComponentManager.h"
#include "Actors/Player.h"

World::World()
{
	Actor* player = new Actor(this, "Player");
	m_Actors[player->GetName()] = player;
	//player->AddComponent()
}

World::~World()
{
	delete m_pComponentManager;
}

void World::Init()
{
	m_pComponentManager = new ComponentManager();
}

void World::Update(float delta)
{
	m_pComponentManager->Update(delta);
}

void World::Draw(const sf::RenderWindow& window)
{
	m_pComponentManager->Draw(window);
}

Actor* World::GetActorByName(std::string_view name)
{
	return m_Actors[name];
}
