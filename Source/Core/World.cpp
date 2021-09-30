#include "GamePCH.h"
#include "World.h"
#include "Managers/ComponentManager.h"
#include "Managers/ResourceManager.h"
#include "Components/SpriteComponent.h"
#include "Game.h"
#include "Actors/Actor.h"

World::World(Game* pGame)
{
	m_pGame = pGame;
}

World::~World()
{
}

void World::Init()
{
	Actor* player = new Actor(this, "Player");
	m_Actors[player->GetName()] = player;
	player->AddComponent(new SpriteComponent("Ship.png"));
	
	
}

void World::Update(float delta)
{
	for (const auto& [name, actor] : m_Actors)
	{
		actor->Update(delta);
	}
}

void World::Draw(sf::RenderWindow* pWindow)
{
	GetComponentManager()->Draw(pWindow);
}

Actor* World::GetActorByName(std::string_view name)
{
	return m_Actors[name];
}

ComponentManager* World::GetComponentManager()
{
	return m_pGame->GetComponentManager();
}

ResourceManager* World::GetResourceManager()
{
	return m_pGame->GetResourceManager();
}
