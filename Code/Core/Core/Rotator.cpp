#include "Rotator.h"

#include "Core/Vector.h"

constexpr Rotator::Rotator(const vec3& value) noexcept
	: pitch(value.x)
	, yaw(value.y)
	, roll(value.z)
{
}

const Rotator Rotator::Zero(0.f);