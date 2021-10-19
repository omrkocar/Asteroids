#pragma once

namespace Random
{
	// #todo: specify allowed types
	template<typename Type>
	Type Range(const Type& min, const Type& max);

	template<>
	inline float Range<float>(const float& min, const float& max);
}

#include "Random.inl"