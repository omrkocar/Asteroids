#pragma once

namespace Random
{
	template<typename Type>
	Type Range(const Type& min, const Type& max);

	template<>
	inline float Range<float>(const float& min, const float& max);
	inline float Float();
}

#include "Random.inl"