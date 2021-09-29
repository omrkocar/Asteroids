#pragma once

class Component;

namespace sf
{
	class RenderWindow;
}

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	void Draw(const sf::RenderWindow& window);
	void Update(float deltaTime);

	void AddComponent(Component* pComponent);
	void RemoveComponent(Component* pComponent);

	std::vector<Component*>& GetComponentList(const char* type);

protected:
	std::map<const char*, std::vector<Component*>> m_Components;
	
};