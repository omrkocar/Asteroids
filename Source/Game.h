#pragma once

class World;

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Destroy();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow* window);

private:
	World* m_pWorld = nullptr;
};

