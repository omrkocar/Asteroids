#pragma once

namespace sf
{
	class RenderWindow;
}

class Component;
class World;

class Actor
{
public:
	Actor(World* pWorld, const char* name);
	virtual ~Actor();

public:
	virtual void Init() {}

	virtual void Update(float delta) {}
	virtual void Draw(sf::RenderWindow* window) {}

	World* GetWorld() { return m_pWorld; }

	void AddComponent(Component* pComponent);

	const char* GetName() { return m_Name; }

	Component* GetFirstComponentOfType(const char* type);

protected:
	World* m_pWorld = nullptr;

	const char* m_Name = nullptr;

	std::vector<Component*> m_Components;
};