#include "GamePCH.h"
#include "Player.h"


Player::Player(const char* name)
	: Actor(m_pWorld, name)
{
	m_Name = name;
}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update(float delta)
{

}

void Player::Draw(sf::RenderWindow* window)
{
	
}

