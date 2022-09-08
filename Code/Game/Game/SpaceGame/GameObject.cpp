#include "GamePCH.h"
#include "GameObject.h"

namespace Game
{
	void GameObject::Init()
	{

	}

	void GameObject::OnUpdate(const Saz::GameTime& gameTime)
	{
		Bound.m_Upper = m_Position + m_Size / 2.0f - 0.1f;
		Bound.m_Lower = m_Position - m_Size / 2.0f - 0.1f;
	}

	void GameObject::OnRender()
	{

	}

	void GameObject::OnImGuiRender()
	{

	}
}