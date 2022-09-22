#pragma once

#include "glm/glm.hpp"

namespace Game
{
	// Axis-Aligned Bounding Box struct
	struct AABB
	{
		AABB(const glm::vec2& m_Lower, const glm::vec2& m_Upper);
		AABB();
		~AABB() = default;

		glm::vec2 Center() const;
		glm::vec2 GetDimensions() const;
		glm::vec2 GetHalfDimensions() const;

		bool Intersects(const AABB& other) const;
		bool Contains(const AABB& other) const;

		glm::vec2 m_Lower;
		glm::vec2 m_Upper;
	};
}
