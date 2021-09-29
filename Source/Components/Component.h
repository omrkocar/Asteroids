#pragma once

class Actor;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init() {};
	virtual void Update(float deltaTime) {};
	virtual const char* GetType() = 0;

	void SetOwner(Actor* pOwner);
	Actor* GetOwner();

	void Activate();
	void Deactivate();
	void ToggleActive();
	const bool IsActive() const;

protected:
	bool m_IsActive = false;
	Actor* m_pOwner = nullptr;
};