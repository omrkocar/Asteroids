#pragma once

class ComponentManager;

namespace sf
{
	class RenderWindow;
}

class Actor;

class World
{
public:
	World();
	~World();

public:
	virtual void Init();

	virtual void Update(float delta);

	virtual void Draw(const sf::RenderWindow& window);

	ComponentManager* GetComponentManager() { return m_pComponentManager; }

	Actor* GetActorByName(std::string_view name);

private:
	ComponentManager* m_pComponentManager = nullptr;

	std::map<std::string_view, Actor*> m_Actors;
};