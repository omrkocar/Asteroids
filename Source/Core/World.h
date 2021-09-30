#pragma once

class ComponentManager;
class ResourceManager;
class Game;

namespace sf
{
	class RenderWindow;
}

class Actor;

class World
{
public:
	World(Game* pGame);
	~World();

public:
	virtual void Init();

	virtual void Update(float delta);

	virtual void Draw(sf::RenderWindow* pWindow);

	Actor* GetActorByName(std::string_view name);

	ComponentManager* GetComponentManager();
	ResourceManager* GetResourceManager();

	Game* GetGame() { return m_pGame; }

	//template<typename T>
	//T* GetManager()
	//{
	//	m_pGame->GetManager<T>();
	//}

private:

	Game* m_pGame = nullptr;

	std::map<std::string_view, Actor*> m_Actors;
};