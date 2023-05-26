//
// Copyright (c) 2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Vector.h"

inline Quaternion Quaternion::FromRotator(const Vector3& value) noexcept
{
	Quaternion quaternion = Quaternion::Identity;
	quaternion = quaternion * Quaternion::FromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), value.x);
	quaternion = Quaternion::FromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), value.y) * quaternion;
	quaternion = Quaternion::FromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), value.z) * quaternion;
	return quaternion;
}

inline Quaternion Quaternion::FromAxisAngle(const Vector3& axis, const float degrees) noexcept
{
	const float radians = Math::ToRadians(degrees) * 0.5f;
	return Quaternion(axis * sinf(radians), cosf(radians));
}

inline Vector3 operator*(const Vector3& vec, const Quaternion& quat) noexcept
{
	const Vector3 quatv(quat.x, quat.y, quat.z);
	return quatv * (2.0f * quatv.Dot(vec))
		+ vec * (quat.w * quat.w - quatv.Dot(quatv))
		+ quatv.Cross(vec) * (2.0f * quat.w);
}

inline Quaternion Quaternion::Lerp(Quaternion start, Quaternion end, float perc)
{
	float cosom = start.Dot(end);

	if (cosom < 0.0f)
	{
		end.x = -end.x;
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	float sclp, sclq;
	sclp = 1.0f - perc;
	sclq = perc;

	Quaternion result = start * sclp + end * sclq;
	return result;
}

inline Quaternion Quaternion::Slerp(Quaternion start, Quaternion end, float perc)
{
	float cosom = start.Dot(end);

	if (cosom < 0.0f)
	{
		cosom = -cosom;
		end.x = -end.x;   
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	float sclp, sclq;
	if ((1.0f - cosom) > 0.0001f) 
	{
		float omega, sinom;
		omega = acosf(cosom);
		sinom = sinf(omega);
		sclp = sinf((1.0f - perc) * omega) / sinom;
		sclq = sinf(perc * omega) / sinom;
	}
	else
	{
		sclp = 1.0f - perc;
		sclq = perc;
	}

	Quaternion result = start * sclp + end * sclq;
	return result;
}
