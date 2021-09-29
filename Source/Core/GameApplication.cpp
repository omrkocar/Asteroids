#include "GamePCH.h"
#include "GameApplication.h"
#include "Game.h"

GameApplication::GameApplication()
{
	m_pClock = new sf::Clock();

	m_pWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Game");

	m_pGame = new Game();
}

GameApplication::~GameApplication()
{
	delete m_pClock;
	delete m_pWindow;
	delete m_pGame;
}

bool GameApplication::Init()
{
	m_pGame->Init();
	return true;
}

void GameApplication::Destroy()
{
	m_pGame->Destroy();
}

bool GameApplication::Update()
{
	sf::Time delta = m_pClock->restart();
	m_pGame->Update(delta.asSeconds());
	return true;
}

bool GameApplication::Draw()
{
	if (!m_pWindow->isOpen())
		return false;

	sf::Event event;
	while (m_pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_pWindow->close();
			return false;
		}
	}

	m_pWindow->clear(sf::Color(200, 200, 100, 255));
	m_pGame->Draw(m_pWindow);
	m_pWindow->display();

	return true;
}
