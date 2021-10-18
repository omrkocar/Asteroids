#pragma once

#include <Core/Vector.h>

namespace component
{
	struct TransformComponent
	{
		vec3 m_Position = vec3::Zero();
		vec3 m_Rotation = vec3::Zero();
		vec3 m_Scale = vec3::One();
	};
}
