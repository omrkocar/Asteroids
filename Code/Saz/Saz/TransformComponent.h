#pragma once

#include <Core/Vector.h>
#include <Core/Rotator.h>

namespace component
{
	struct TransformComponent
	{
		vec3 m_Position = vec3::Zero();
		Rotator m_Rotation = Rotator::Zero;
		vec3 m_Scale = vec3::One();
	};
}
