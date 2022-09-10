#pragma once

#include "glm/glm.hpp"

namespace component
{
	struct TransformComponent
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		float Rotation;
		glm::vec2 Scale{ 1.0f, 1.0f };
	};
}
