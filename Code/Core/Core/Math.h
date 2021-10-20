#pragma once

#include <math.h>

constexpr float LARGE_FLOAT = 9999999.0f;
constexpr float EPSILON = 0.0000001f;

constexpr float PI_TWO = 6.2831853071795864769252867665590f;
constexpr float PI = 3.1415926535897932384626433832795f;
constexpr float PI_HALF = 1.5707963267948966192313216916398f;

namespace Math
{
	template<typename T>
	inline constexpr T Clamp(const T& value, const T& min, const T& max)
	{
		return value < min ? min : value > max ? max : value;
	}

	template <class T> 
	inline T Clamped(T value, T min, T max)
	{
		T temp = value;

		if (temp < min)
			temp = min;

		if (temp > max)
			temp = max;

		return temp;
	}

	inline bool fequal(const float a, const float b, const float epsilon = EPSILON)
	{
		return fabs(a - b) <= epsilon;
	}

	template<typename T>
	inline constexpr float Sqr(const T value)
	{
		return value * value;
	}

	inline float Sqrt(const float value)
	{
		return sqrtf(value);
	}

	template<typename T>
	inline constexpr T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template<typename T>
	inline constexpr T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	inline constexpr float ToDegrees(float radians)
	{
		return radians * 57.2958f;
	}

	inline constexpr float ToRadians(float degrees)
	{
		return degrees * 0.0174533f;
	}

	template <class T> void Swap(T& v1, T& v2)
	{
		T temp = v1;
		v1 = v2;
		v2 = temp;
	}


	template <class T> 
	inline void IncreaseIfBigger(T& value, T newvalue)
	{
		if (newvalue > value)
			value = newvalue;
	}

	template <class T> 
	inline void DecreaseIfLower(T& value, T newvalue, bool treatzeroasinfinite = false)
	{
		if (treatzeroasinfinite && value == 0)
			value = newvalue;

		if (newvalue < value)
			value = newvalue;
	}
}
