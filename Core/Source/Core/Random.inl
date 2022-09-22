#pragma once

#include <random>

template<typename Type>
Type Random::Range<Type>(const Type& min, const Type& max)
{
	if (min == max)
		return min;

	const Type r = std::rand() % (max - min + 1);
	return r + min;
}

template<>
float Random::Range<float>(const float& min, const float& max)
{
	if (min == max)
		return min;

	const float r = std::rand() / (float)RAND_MAX;
	return (r * (max - min)) + min;
}

float Random::Float()
{
	return rand() / (RAND_MAX + 1.);
}