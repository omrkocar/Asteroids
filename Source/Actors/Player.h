#pragma once

#include "Actor.h"

class Player : public Actor
{
public:
	Player(const char* name);
	~Player();

	virtual void Init() override;

	virtual void Update(float delta) override;
	virtual void Draw(sf::RenderWindow* window) override;
};