#pragma once

#include "Core/Math.h"
#include <assert.h>

class Vector2
{
public:
	float x;
	float y;

public:
	Vector2() {}
	Vector2(float nxy) { x = nxy; y = nxy; }
	Vector2(float nx, float ny) { x = nx; y = ny; }
	//virtual ~Vector2() {}

	static const Vector2 Zero() { return Vector2(0.0f, 0.0f); }
	static const Vector2 One() { return Vector2(1.0f, 1.0f); }
	static const Vector2 Right() { return Vector2(1.0f, 0.0f); }
	static const Vector2 Up() { return Vector2(0.0f, 1.0f); }

	inline void Set(float nx, float ny) { x = nx; y = ny; }
	inline float LengthSquared() const { return x * x + y * y; }
	inline float Length() const { return sqrtf(x * x + y * y); }
	inline float DistanceFrom(const Vector2 o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }

	inline Vector2 GetNormalized() const { float len = Length(); if (Math::fequal(len, 0)) return Vector2(x, y); len = 1.0f / len; return Vector2(x * len, y * len); }
	inline Vector2 Normalize() { float len = Length(); if (!Math::fequal(len, 0)) { x /= len; y /= len; } return *this; }
	inline void Absolute() { if (x < 0) x *= -1; if (y < 0) y *= -1; }
	inline Vector2 GetAbsolute() const { return Vector2(x < 0 ? -x : x, y < 0 ? -y : y); }
	inline float Dot(const Vector2& o) const { return x * o.x + y * o.y; }
	inline Vector2 Add(const Vector2& o) const { return Vector2(this->x + o.x, this->y + o.y); }
	inline Vector2 Sub(const Vector2& o) const { return Vector2(this->x - o.x, this->y - o.y); }
	inline Vector2 Scale(const float o) const { return Vector2(this->x * o, this->y * o); }

	inline Vector2 WithX(float x) const { return Vector2(x, this->y); }
	inline Vector2 WithY(float y) const { return Vector2(this->x, y); }

	inline bool operator ==(const Vector2& o) const { return Math::fequal(this->x, o.x) && Math::fequal(this->y, o.y); }
	inline bool operator !=(const Vector2& o) const { return !Math::fequal(this->x, o.x) || !Math::fequal(this->y, o.y); }

	inline Vector2 operator -() const { return Vector2(-this->x, -this->y); }
	inline Vector2 operator *(const float o) const { return Vector2(this->x * o, this->y * o); }
	inline Vector2 operator /(const float o) const { return Vector2(this->x / o, this->y / o); }
	inline Vector2 operator +(const float o) const { return Vector2(this->x + o, this->y + o); }
	inline Vector2 operator -(const float o) const { return Vector2(this->x - o, this->y - o); }
	inline Vector2 operator *(const Vector2& o) const { return Vector2(this->x * o.x, this->y * o.y); }
	inline Vector2 operator /(const Vector2& o) const { return Vector2(this->x / o.x, this->y / o.y); }
	inline Vector2 operator +(const Vector2& o) const { return Vector2(this->x + o.x, this->y + o.y); }
	inline Vector2 operator -(const Vector2& o) const { return Vector2(this->x - o.x, this->y - o.y); }

	inline Vector2 operator *=(const float o) { this->x *= o; this->y *= o; return *this; }
	inline Vector2 operator /=(const float o) { this->x /= o; this->y /= o; return *this; }
	inline Vector2 operator +=(const float o) { this->x += o; this->y += o; return *this; }
	inline Vector2 operator -=(const float o) { this->x -= o; this->y -= o; return *this; }
	inline Vector2 operator *=(const Vector2& o) { this->x *= o.x; this->y *= o.y; return *this; }
	inline Vector2 operator /=(const Vector2& o) { this->x /= o.x; this->y /= o.y; return *this; }
	inline Vector2 operator +=(const Vector2& o) { this->x += o.x; this->y += o.y; return *this; }
	inline Vector2 operator -=(const Vector2& o) { this->x -= o.x; this->y -= o.y; return *this; }

	float& operator[] (int i) { assert(i >= 0 && i < 2); return *(&x + i); }
};

inline Vector2 operator *(float scalar, const Vector2& vec) { return Vector2(scalar * vec.x, scalar * vec.y); }
inline Vector2 operator /(float scalar, const Vector2& vec) { return Vector2(scalar / vec.x, scalar / vec.y); }
inline Vector2 operator +(float scalar, const Vector2& vec) { return Vector2(scalar + vec.x, scalar + vec.y); }
inline Vector2 operator -(float scalar, const Vector2& vec) { return Vector2(scalar - vec.x, scalar - vec.y); }

class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	Vector3() {}
	Vector3(float nxyz) { x = nxyz; y = nxyz; z = nxyz; }
	Vector3(float nx, float ny) { x = nx; y = ny; z = 0; }
	Vector3(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }
	Vector3(Vector2 v2) { x = v2.x; y = v2.y; z = 0; }
	Vector3(Vector2 v2, float nz) { x = v2.x; y = v2.y; z = nz; }
	//virtual ~Vector3() {}

	static const Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
	static const Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
	static const Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
	static const Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
	static const Vector3 In() { return Vector3(0.0f, 0.0f, 1.0f); }

	inline Vector2 XY() const { return Vector2(x, y); }
	inline Vector2 XZ() const { return Vector2(x, z); }

	inline void Set(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }
	inline float LengthSquared() const { return x * x + y * y + z * z; }
	inline float Length() const { return sqrtf(x * x + y * y + z * z); }
	inline float DistanceFrom(const Vector3 o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z)); }

	inline Vector3 GetNormalized() const { float len = Length(); if (Math::fequal(len, 0)) return Vector3(x, y, z); len = 1.0f / len; return Vector3(x * len, y * len, z * len); }
	inline Vector3 Normalize() { float len = Length(); if (!Math::fequal(len, 0)) { x /= len; y /= len; z /= len; } return *this; }
	inline Vector3 Cross(const Vector3& o) const { return Vector3((y * o.z - z * o.y), (z * o.x - x * o.z), (x * o.y - y * o.x)); }
	inline float Dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }
	inline Vector3 Add(const Vector3& o) const { return Vector3(this->x + o.x, this->y + o.y, this->z + o.z); }
	inline Vector3 Sub(const Vector3& o) const { return Vector3(this->x - o.x, this->y - o.y, this->z - o.z); }
	inline Vector3 Scale(const float o) const { return Vector3(this->x * o, this->y * o, this->z * o); }
	inline Vector3 MultiplyComponents(const Vector3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3 DivideComponents(const Vector3& o) const { return Vector3(this->x / o.x, this->y / o.y, this->z / o.z); }
	//inline Vector3 MultiplyComponents(const ivec3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }

	inline Vector3 WithX(float x) const { return Vector3(x, this->y, this->z); }
	inline Vector3 WithY(float y) const { return Vector3(this->x, y, this->z); }
	inline Vector3 WithZ(float z) const { return Vector3(this->x, this->y, z); }

	inline bool operator ==(const Vector3& o) const { return Math::fequal(this->x, o.x) && Math::fequal(this->y, o.y) && Math::fequal(this->z, o.z); }
	inline bool operator !=(const Vector3& o) const { return !Math::fequal(this->x, o.x) || !Math::fequal(this->y, o.y) || !Math::fequal(this->z, o.z); }

	inline Vector3 operator -() const { return Vector3(-this->x, -this->y, -this->z); }
	inline Vector3 operator *(const float o) const { return Vector3(this->x * o, this->y * o, this->z * o); }
	inline Vector3 operator /(const float o) const { return Vector3(this->x / o, this->y / o, this->z / o); }
	inline Vector3 operator +(const float o) const { return Vector3(this->x + o, this->y + o, this->z + o); }
	inline Vector3 operator -(const float o) const { return Vector3(this->x - o, this->y - o, this->z - o); }
	inline Vector3 operator *(const Vector3& o) const { return Vector3(this->x * o.x, this->y * o.y, this->z * o.z); }
	inline Vector3 operator /(const Vector3& o) const { return Vector3(this->x / o.x, this->y / o.y, this->z / o.z); }
	inline Vector3 operator +(const Vector3& o) const { return Vector3(this->x + o.x, this->y + o.y, this->z + o.z); }
	inline Vector3 operator -(const Vector3& o) const { return Vector3(this->x - o.x, this->y - o.y, this->z - o.z); }

	inline Vector3 operator *=(const float o) { this->x *= o; this->y *= o; this->z *= o; return *this; }
	inline Vector3 operator /=(const float o) { this->x /= o; this->y /= o; this->z /= o; return *this; }
	inline Vector3 operator +=(const float o) { this->x += o; this->y += o; this->z += o; return *this; }
	inline Vector3 operator -=(const float o) { this->x -= o; this->y -= o; this->z -= o; return *this; }
	inline Vector3 operator *=(const Vector3& o) { this->x *= o.x; this->y *= o.y; this->z *= o.z; return *this; }
	inline Vector3 operator /=(const Vector3& o) { this->x /= o.x; this->y /= o.y; this->z /= o.z; return *this; }
	inline Vector3 operator +=(const Vector3& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
	inline Vector3 operator -=(const Vector3& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }

	float& operator[] (int i) { assert(i >= 0 && i < 3); return *(&x + i); }

	//inline void operator =(const Vector2& o) { x = o.x; y = o.y; z = 0; } // couldn't make this work, used a constructor instead.
};

inline Vector3 operator *(float scalar, const Vector3& vec) { return Vector3(scalar * vec.x, scalar * vec.y, scalar * vec.z); }
inline Vector3 operator /(float scalar, const Vector3& vec) { return Vector3(scalar / vec.x, scalar / vec.y, scalar / vec.z); }
inline Vector3 operator +(float scalar, const Vector3& vec) { return Vector3(scalar + vec.x, scalar + vec.y, scalar + vec.z); }
inline Vector3 operator -(float scalar, const Vector3& vec) { return Vector3(scalar - vec.x, scalar - vec.y, scalar - vec.z); }

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	Vector4() {}
	Vector4(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
	Vector4(Vector2 vec, float nz, float nw) { x = vec.x; y = vec.y; z = nz; w = nw; }
	Vector4(Vector3 vec, float nw) { x = vec.x; y = vec.y; z = vec.z; w = nw; }
	//virtual ~Vector4() {}

	inline Vector3 XYZ() { return Vector3(x, y, z); }

	inline void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
	inline float LengthSquared() const { return x * x + y * y + z * z + w * w; }
	inline float Length() const { return sqrtf(x * x + y * y + z * z + w * w); }
	inline float DistanceFrom(const Vector4 o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z) + (w - o.w) * (w - o.w)); }

	inline Vector4 GetNormalized() const { float len = Length(); if (Math::fequal(len, 0)) return Vector4(x, y, z, w); len = 1.0f / len; return Vector4(x * len, y * len, z * len, w * len); }
	inline Vector4 Normalize() { float len = Length(); if (!Math::fequal(len, 0)) { x /= len; y /= len; z /= len; w /= len; } return *this; }
	//Vector4 CrossProduct(const Vector4& b, const Vector4& c)
	// from http://www.gamedev.net/topic/269241-4d-vec-class/
	//  not sure if right and have no use for it.
	//{
	//    //Precompute some 2x2 matrix determinants for speed
	//    float Pxy = b.x*c.y - c.x*b.y;
	//    float Pxz = b.x*c.z - c.x*b.z;
	//    float Pxw = b.x*c.w - c.x*b.w;
	//    float Pyz = b.y*c.z - c.y*b.z;
	//    float Pyw = b.y*c.w - c.y*b.w;
	//    float Pzw = b.z*c.w - c.z*b.w;
	//    return Vector4(
	//        y*Pzw - z*Pyw + w*Pyz,    //Note the lack of 'x' in this line
	//        z*Pxw - x*Pzw - w*Pxz,    //y, Etc.
	//        x*Pyw - y*Pxw + w*Pxy,
	//        y*Pxz - x*Pyz - z*Pxy
	//        );
	//}
	inline float Dot(const Vector4& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
	inline Vector4 Add(const Vector4& o) const { return Vector4(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
	inline Vector4 Sub(const Vector4& o) const { return Vector4(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }
	inline Vector4 Scale(const float o) const { return Vector4(this->x * o, this->y * o, this->z * o, this->w * o); }
	inline Vector4 MultiplyComponents(const Vector4& o) const { return Vector4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }
	inline Vector4 DivideComponents(const Vector4& o) const { return Vector4(this->x / o.x, this->y / o.y, this->z / o.z, this->w / o.w); }
	//inline Vector4 MultiplyComponents(const ivec4& o) const { return Vector4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }

	inline Vector4 WithX(float x) const { return Vector4(x, this->y, this->z, this->w); }
	inline Vector4 WithY(float y) const { return Vector4(this->x, y, this->z, this->w); }
	inline Vector4 WithZ(float z) const { return Vector4(this->x, this->y, z, this->w); }
	inline Vector4 WithW(float w) const { return Vector4(this->x, this->y, this->z, w); }

	inline bool operator ==(const Vector4& o) const { return Math::fequal(this->x, o.x) && Math::fequal(this->y, o.y) && Math::fequal(this->z, o.z) && Math::fequal(this->w, o.w); }
	inline bool operator !=(const Vector4& o) const { return !Math::fequal(this->x, o.x) || !Math::fequal(this->y, o.y) || !Math::fequal(this->z, o.z) || !Math::fequal(this->w, o.w); }

	inline Vector4 operator -() const { return Vector4(-this->x, -this->y, -this->z, -this->w); }
	inline Vector4 operator *(const float o) const { return Vector4(this->x * o, this->y * o, this->z * o, this->w * o); }
	inline Vector4 operator /(const float o) const { return Vector4(this->x / o, this->y / o, this->z / o, this->w / o); }
	inline Vector4 operator +(const float o) const { return Vector4(this->x + o, this->y + o, this->z + o, this->w + o); }
	inline Vector4 operator -(const float o) const { return Vector4(this->x - o, this->y - o, this->z - o, this->w - o); }
	inline Vector4 operator *(const Vector4& o) const { return Vector4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }
	inline Vector4 operator /(const Vector4& o) const { return Vector4(this->x / o.x, this->y / o.y, this->z / o.z, this->w / o.w); }
	inline Vector4 operator +(const Vector4& o) const { return Vector4(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
	inline Vector4 operator -(const Vector4& o) const { return Vector4(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }

	float& operator[] (int i) { assert(i >= 0 && i < 4); return *(&x + i); }
};

inline Vector4 operator *(float scalar, const Vector4& vec) { return Vector4(scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w); }
inline Vector4 operator /(float scalar, const Vector4& vec) { return Vector4(scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w); }
inline Vector4 operator +(float scalar, const Vector4& vec) { return Vector4(scalar + vec.x, scalar + vec.y, scalar + vec.z, scalar + vec.w); }
inline Vector4 operator -(float scalar, const Vector4& vec) { return Vector4(scalar - vec.x, scalar - vec.y, scalar - vec.z, scalar - vec.w); }

class Vector2Int
{
public:
	int x;
	int y;

public:
	Vector2Int() {}
	Vector2Int(int nx, int ny) { x = nx; y = ny; }
	Vector2Int(Vector2 o) { x = (int)o.x; y = (int)o.y; }
	//virtual ~Vector2Int() {}

	static const Vector2 Zero() { return Vector2(0, 0); }
	static const Vector2 One() { return Vector2(1, 1); }

	inline void Set(int nx, int ny) { x = nx; y = ny; }
	inline float LengthSquared() const { return (float)x * x + y * y; }
	inline float Length() const { return sqrtf((float)x * x + y * y); }
	inline float DistanceFrom(const Vector2Int o) const { return sqrtf(((float)x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }

	//inline Vector2Int Normalize() const {float len = Length(); if( Math::fequal(len,0) ) return Vector2Int(x,y); len = 1.0f/len; return Vector2Int(x*len, y*len);}

	inline Vector2Int WithX(int x) const { return Vector2Int(x, this->y); }
	inline Vector2Int WithY(int y) const { return Vector2Int(this->x, y); }

	inline bool operator ==(const Vector2Int& o) const { return this->x == o.x && this->y == o.y; }
	inline bool operator !=(const Vector2Int& o) const { return this->x != o.x || this->y != o.y; }

	inline Vector2Int operator -() const { return Vector2Int(-this->x, -this->y); }
	inline Vector2 operator *(const float o) const { return Vector2(this->x * o, this->y * o); }
	inline Vector2 operator /(const float o) const { return Vector2(this->x / o, this->y / o); }
	inline Vector2 operator +(const float o) const { return Vector2(this->x + o, this->y + o); }
	inline Vector2 operator -(const float o) const { return Vector2(this->x - o, this->y - o); }
	inline Vector2Int operator *(const int o) const { return Vector2Int(this->x * o, this->y * o); }
	inline Vector2Int operator /(const int o) const { return Vector2Int(this->x / o, this->y / o); }
	inline Vector2Int operator +(const int o) const { return Vector2Int(this->x + o, this->y + o); }
	inline Vector2Int operator -(const int o) const { return Vector2Int(this->x - o, this->y - o); }
	inline Vector2 operator *(const Vector2& o) const { return Vector2(this->x * o.x, this->y * o.y); }
	inline Vector2 operator /(const Vector2& o) const { return Vector2(this->x / o.x, this->y / o.y); }
	inline Vector2 operator +(const Vector2& o) const { return Vector2(this->x + o.x, this->y + o.y); }
	inline Vector2 operator -(const Vector2& o) const { return Vector2(this->x - o.x, this->y - o.y); }
	inline Vector2Int operator *(const Vector2Int& o) const { return Vector2Int(this->x * o.x, this->y * o.y); }
	inline Vector2Int operator /(const Vector2Int& o) const { return Vector2Int(this->x / o.x, this->y / o.y); }
	inline Vector2Int operator +(const Vector2Int& o) const { return Vector2Int(this->x + o.x, this->y + o.y); }
	inline Vector2Int operator -(const Vector2Int& o) const { return Vector2Int(this->x - o.x, this->y - o.y); }

	inline Vector2Int operator *=(const int o) { this->x *= o; this->y *= o; return *this; }
	inline Vector2Int operator /=(const int o) { this->x /= o; this->y /= o; return *this; }
	inline Vector2Int operator +=(const int o) { this->x += o; this->y += o; return *this; }
	inline Vector2Int operator -=(const int o) { this->x -= o; this->y -= o; return *this; }
	inline Vector2Int operator *=(const Vector2Int& o) { this->x *= o.x; this->y *= o.y; return *this; }
	inline Vector2Int operator /=(const Vector2Int& o) { this->x /= o.x; this->y /= o.y; return *this; }
	inline Vector2Int operator +=(const Vector2Int& o) { this->x += o.x; this->y += o.y; return *this; }
	inline Vector2Int operator -=(const Vector2Int& o) { this->x -= o.x; this->y -= o.y; return *this; }

	int& operator[] (int i) { assert(i >= 0 && i < 2); return *(&x + i); }
};

inline Vector2Int operator *(int scalar, const Vector2Int& vec) { return Vector2Int(scalar * vec.x, scalar * vec.y); }
inline Vector2Int operator /(int scalar, const Vector2Int& vec) { return Vector2Int(scalar / vec.x, scalar / vec.y); }
inline Vector2Int operator +(int scalar, const Vector2Int& vec) { return Vector2Int(scalar + vec.x, scalar + vec.y); }
inline Vector2Int operator -(int scalar, const Vector2Int& vec) { return Vector2Int(scalar - vec.x, scalar - vec.y); }