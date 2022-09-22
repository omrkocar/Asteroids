#include "GamePCH.h"

#include "AABB.h"

namespace Game
{
	AABB::AABB(const glm::vec2& m_Lower, const glm::vec2& m_Upper)
		: m_Lower(m_Lower),
		m_Upper(m_Upper)
	{

	}

	AABB::AABB()
		: m_Lower(0.0f, 0.0f),
		m_Upper(1.0f, 1.0f)
	{

	}

	glm::vec2 AABB::Center() const
	{
		return m_Lower + GetHalfDimensions();
	}

	glm::vec2 AABB::GetDimensions() const
	{
		return m_Upper - m_Lower;
	}

	glm::vec2 AABB::GetHalfDimensions() const
	{
		return GetDimensions() * 0.5f;
	}


	bool AABB::Intersects(const AABB& other) const
	{
		if (m_Upper.x < other.m_Lower.x)
			return false;

		if (m_Upper.y < other.m_Lower.y)
			return false;

		if (m_Lower.x > other.m_Upper.x)
			return false;

		if (m_Lower.y > other.m_Upper.y)
			return false;

		return true;
	}

	bool AABB::Contains(const AABB& other) const
	{
		if (other.m_Lower.x >= m_Lower.x &&
			other.m_Upper.x <= m_Upper.x &&
			other.m_Lower.y >= m_Lower.y &&
			other.m_Upper.y <= m_Upper.y)
			return true;

		return false;
	}

}

