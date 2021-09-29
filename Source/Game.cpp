#include "GamePCH.h"

#include "Game.h"
#include "Core/World.h"

Game::Game()
{
	m_pWorld = new World();
}

Game::~Game()
{
	delete m_pWorld;
}

void Game::Init()
{
	m_pWorld->Init();
}

void Game::Destroy()
{

}

void Game::Update(float deltaTime)
{
	m_pWorld->Update(deltaTime);
}

void Game::Draw(sf::RenderWindow* window)
{
	m_pWorld->Draw(*window);
}
