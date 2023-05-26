//
// Copyright (c) 2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math.h"
#include "Vector.h"

namespace Saz
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

	public:
		Quaternion() {}
		Quaternion(Vector3 nv, float nw) { x = nv.x; y = nv.y; z = nv.z; w = nw; }
		Quaternion(Vector4 nv) { x = nv.x; y = nv.y; z = nv.z; w = nv.w; }
		Quaternion(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }

		inline void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
		inline float LengthSquared() const { return x * x + y * y + z * z + w * w; }
		inline float Length() const { return sqrtf(x * x + y * y + z * z + w * w); }

		inline Quaternion GetNormalized() const { float len = Length(); if (Math::fequal(len, 0)) return Quaternion(x, y, z, w); len = 1.0f / len; return Quaternion(x * len, y * len, z * len, w * len); }
		inline Quaternion Normalize() { float len = Length(); if (!Math::fequal(len, 0)) { x /= len; y /= len; z /= len; w /= len; } return *this; }
		inline float Dot(const Quaternion& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
		inline Quaternion GetConjugate() { return Quaternion(-x, -y, -z, w); }
		inline void Conjugate() { x *= -1; y *= -1; z *= -1; }
		inline Quaternion GetInverse() { return GetConjugate() * 1.0f / LengthSquared(); }
		inline void Inverse() { Conjugate(); *this *= 1.0f / LengthSquared(); }
		static Quaternion FromRotator(const Vector3& value) noexcept;
		static Quaternion FromAxisAngle(const Vector3& axis, const float degrees) noexcept;

		inline bool operator ==(const Quaternion& o) const { return Math::fequal(this->x, o.x) && Math::fequal(this->y, o.y) && Math::fequal(this->z, o.z) && Math::fequal(this->w, o.w); }
		inline bool operator !=(const Quaternion& o) const { return !Math::fequal(this->x, o.x) || !Math::fequal(this->y, o.y) || !Math::fequal(this->z, o.z) || !Math::fequal(this->w, o.w); }

		inline Quaternion operator *(const float o) const { return Quaternion(this->x * o, this->y * o, this->z * o, this->w * o); }
		inline Quaternion operator /(const float o) const { return Quaternion(this->x / o, this->y / o, this->z / o, this->w / o); }
		inline Quaternion operator +(const float o) const { return Quaternion(this->x + o, this->y + o, this->z + o, this->w + o); }
		inline Quaternion operator -(const float o) const { return Quaternion(this->x - o, this->y - o, this->z - o, this->w - o); }
		inline Quaternion operator +(const Quaternion& o) const { return Quaternion(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
		inline Quaternion operator -(const Quaternion& o) const { return Quaternion(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }
		inline Quaternion operator *(const Quaternion& o) const
		{
			return Quaternion(
				w * o.x + x * o.w + y * o.z - z * o.y,
				w * o.y + y * o.w + z * o.x - x * o.z,
				w * o.z + z * o.w + x * o.y - y * o.x,
				w * o.w - x * o.x - y * o.y - z * o.z);
		}
		inline Quaternion operator /(const Quaternion& o) const { Quaternion temp = o; return *this * temp.GetInverse(); }

		inline Quaternion operator *=(const float o) { this->x *= o; this->y *= o; this->z *= o; this->w *= o; return *this; }
		inline Quaternion operator /=(const float o) { this->x /= o; this->y /= o; this->z /= o; this->w /= o; return *this; }
		inline Quaternion operator +=(const float o) { this->x += o; this->y += o; this->z += o; this->w += o; return *this; }
		inline Quaternion operator -=(const float o) { this->x -= o; this->y -= o; this->z -= o; this->w -= o; return *this; }
		inline Quaternion operator +=(const Quaternion& o) { this->x += o.x; this->y += o.y; this->z += o.z; this->w += o.w; return *this; }
		inline Quaternion operator -=(const Quaternion& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; this->w -= o.w; return *this; }


		static Quaternion Lerp(Quaternion start, Quaternion end, float perc);
		static Quaternion Slerp(Quaternion start, Quaternion end, float perc);

		static const Quaternion Identity;

	};

	Vector3 operator*(const Vector3& vec, const Quaternion& quat) noexcept;

	#include <Core/Quaternion.inl>
}
