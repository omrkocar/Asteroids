#pragma once

#include <math.h>

constexpr float LARGE_FLOAT = 9999999.0f;
constexpr float EPSILON = 0.0000001f;

constexpr float PI_TWO = 6.2831853071795864769252867665590f;
constexpr float PI_ONE = 3.1415926535897932384626433832795f;
constexpr float PI_HALF = 1.5707963267948966192313216916398f;

constexpr float SQUARE_ROOT_TWO = 1.4142135623730950488016887242097f;

namespace Math
{
	template<typename Type>
	inline constexpr Type Clamp(const Type& value, const Type& min, const Type& max)
	{
		return value < min ? min : value > max ? max : value;
	}

	template <class MyType> 
	inline MyType Clamped(MyType value, MyType min, MyType max)
	{
		MyType temp = value;

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

	template<typename Type>
	inline constexpr float Sqr(const Type value)
	{
		return value * value;
	}

	inline float Sqrt(const float value)
	{
		return sqrtf(value);
	}

	template<typename Type>
	inline constexpr Type Min(const Type& a, const Type& b)
	{
		return (a < b) ? a : b;
	}

	template<typename Type>
	inline constexpr Type Max(const Type& a, const Type& b)
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

	template <class MyType> void Swap(MyType& v1, MyType& v2)
	{
		MyType temp = v1;
		v1 = v2;
		v2 = temp;
	}


	template <class MyType> 
	inline void IncreaseIfBigger(MyType& value, MyType newvalue)
	{
		if (newvalue > value)
			value = newvalue;
	}

	template <class MyType> 
	inline void DecreaseIfLower(MyType& value, MyType newvalue, bool treatzeroasinfinite = false)
	{
		if (treatzeroasinfinite && value == 0)
			value = newvalue;

		if (newvalue < value)
			value = newvalue;
	}
}
