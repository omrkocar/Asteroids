#pragma once

#include <cstring>
#include <utility>

#include "Core/Quaternion.h"
#include "Core/Vector.h"

constexpr Matrix::Matrix() noexcept
	: m_Data()
{
}

constexpr Matrix::Matrix(
	const float v00, const float v01, const float v02, const float v03,
	const float v10, const float v11, const float v12, const float v13,
	const float v20, const float v21, const float v22, const float v23,
	const float v30, const float v31, const float v32, const float v33) noexcept
	: m_Data{ { v00, v01, v02, v03 }, { v10, v11, v12, v13 }, { v20, v21, v22, v23 }, { v30, v31, v32, v33 } }
{
}

constexpr Matrix::Matrix(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3) noexcept
	: m_Data{ v0.x, v0.y, v0.z, v0.w, v1.x, v1.y, v1.z, v1.w, v2.x, v2.y, v2.z, v2.w, v3.x, v3.y, v3.z, v3.w }
{
}

inline Matrix::Matrix(const vec3& translate, const Quaternion& rotate, const float scale) noexcept
	: m_Data()
{
	*this = FromRotate(rotate);

	m_Data[0][0] *= scale; m_Data[0][1] *= scale; m_Data[0][2] *= scale;
	m_Data[1][0] *= scale; m_Data[1][1] *= scale; m_Data[1][2] *= scale;
	m_Data[2][0] *= scale; m_Data[2][1] *= scale; m_Data[2][2] *= scale;

	m_Data[3][0] = translate.x;
	m_Data[3][1] = translate.y;
	m_Data[3][2] = translate.z;
}

inline Matrix::Matrix(const vec3& translate, const Quaternion& rotate, const vec3& scale) noexcept
	: m_Data()
{
	*this = FromRotate(rotate);

	m_Data[0][0] *= scale.x; m_Data[0][1] *= scale.x; m_Data[0][2] *= scale.x;
	m_Data[1][0] *= scale.y; m_Data[1][1] *= scale.y; m_Data[1][2] *= scale.y;
	m_Data[2][0] *= scale.z; m_Data[2][1] *= scale.z; m_Data[2][2] *= scale.z;

	m_Data[3][0] = translate.x;
	m_Data[3][1] = translate.y;
	m_Data[3][2] = translate.z;
}

inline bool Matrix::operator==(const Matrix& rhs) const noexcept
{
	return m_Data[0][0] == rhs.m_Data[0][0]
		&& m_Data[0][1] == rhs.m_Data[0][1]
		&& m_Data[0][2] == rhs.m_Data[0][2]
		&& m_Data[0][3] == rhs.m_Data[0][3]

		&& m_Data[1][0] == rhs.m_Data[1][0]
		&& m_Data[1][1] == rhs.m_Data[1][1]
		&& m_Data[1][2] == rhs.m_Data[1][2]
		&& m_Data[1][3] == rhs.m_Data[1][3]

		&& m_Data[2][0] == rhs.m_Data[2][0]
		&& m_Data[2][1] == rhs.m_Data[2][1]
		&& m_Data[2][2] == rhs.m_Data[2][2]
		&& m_Data[2][3] == rhs.m_Data[2][3]

		&& m_Data[3][0] == rhs.m_Data[3][0]
		&& m_Data[3][1] == rhs.m_Data[3][1]
		&& m_Data[3][2] == rhs.m_Data[3][2]
		&& m_Data[3][3] == rhs.m_Data[3][3];
}

inline bool Matrix::operator!=(const Matrix& rhs) const noexcept
{
	return m_Data[0][0] != rhs.m_Data[0][0]
		|| m_Data[0][1] != rhs.m_Data[0][1]
		|| m_Data[0][2] != rhs.m_Data[0][2]
		|| m_Data[0][3] != rhs.m_Data[0][3]

		|| m_Data[1][0] != rhs.m_Data[1][0]
		|| m_Data[1][1] != rhs.m_Data[1][1]
		|| m_Data[1][2] != rhs.m_Data[1][2]
		|| m_Data[1][3] != rhs.m_Data[1][3]

		|| m_Data[2][0] != rhs.m_Data[2][0]
		|| m_Data[2][1] != rhs.m_Data[2][1]
		|| m_Data[2][2] != rhs.m_Data[2][2]
		|| m_Data[2][3] != rhs.m_Data[2][3]

		|| m_Data[3][0] != rhs.m_Data[3][0]
		|| m_Data[3][1] != rhs.m_Data[3][1]
		|| m_Data[3][2] != rhs.m_Data[3][2]
		|| m_Data[3][3] != rhs.m_Data[3][3];
}

inline Matrix Matrix::operator+(const Matrix& rhs) const noexcept
{
	return Matrix{
		m_Data[0][0] + rhs.m_Data[0][0],
		m_Data[0][1] + rhs.m_Data[0][1],
		m_Data[0][2] + rhs.m_Data[0][2],
		m_Data[0][3] + rhs.m_Data[0][3],

		m_Data[1][0] + rhs.m_Data[1][0],
		m_Data[1][1] + rhs.m_Data[1][1],
		m_Data[1][2] + rhs.m_Data[1][2],
		m_Data[1][3] + rhs.m_Data[1][3],

		m_Data[2][0] + rhs.m_Data[2][0],
		m_Data[2][1] + rhs.m_Data[2][1],
		m_Data[2][2] + rhs.m_Data[2][2],
		m_Data[2][3] + rhs.m_Data[2][3],

		m_Data[3][0] + rhs.m_Data[3][0],
		m_Data[3][1] + rhs.m_Data[3][1],
		m_Data[3][2] + rhs.m_Data[3][2],
		m_Data[3][3] + rhs.m_Data[3][3] };
}

inline Matrix Matrix::operator-(const Matrix& rhs) const noexcept
{
	return Matrix{
		m_Data[0][0] - rhs.m_Data[0][0],
		m_Data[0][1] - rhs.m_Data[0][1],
		m_Data[0][2] - rhs.m_Data[0][2],
		m_Data[0][3] - rhs.m_Data[0][3],

		m_Data[1][0] - rhs.m_Data[1][0],
		m_Data[1][1] - rhs.m_Data[1][1],
		m_Data[1][2] - rhs.m_Data[1][2],
		m_Data[1][3] - rhs.m_Data[1][3],

		m_Data[2][0] - rhs.m_Data[2][0],
		m_Data[2][1] - rhs.m_Data[2][1],
		m_Data[2][2] - rhs.m_Data[2][2],
		m_Data[2][3] - rhs.m_Data[2][3],

		m_Data[3][0] - rhs.m_Data[3][0],
		m_Data[3][1] - rhs.m_Data[3][1],
		m_Data[3][2] - rhs.m_Data[3][2],
		m_Data[3][3] - rhs.m_Data[3][3] };
}

inline Matrix Matrix::operator*(const Matrix& rhs) const noexcept
{
	return Matrix{
		m_Data[0][0] * rhs.m_Data[0][0] + m_Data[0][1] * rhs.m_Data[1][0] + m_Data[0][2] * rhs.m_Data[2][0] + m_Data[0][3] * rhs.m_Data[3][0],
		m_Data[0][0] * rhs.m_Data[0][1] + m_Data[0][1] * rhs.m_Data[1][1] + m_Data[0][2] * rhs.m_Data[2][1] + m_Data[0][3] * rhs.m_Data[3][1],
		m_Data[0][0] * rhs.m_Data[0][2] + m_Data[0][1] * rhs.m_Data[1][2] + m_Data[0][2] * rhs.m_Data[2][2] + m_Data[0][3] * rhs.m_Data[3][2],
		m_Data[0][0] * rhs.m_Data[0][3] + m_Data[0][1] * rhs.m_Data[1][3] + m_Data[0][2] * rhs.m_Data[2][3] + m_Data[0][3] * rhs.m_Data[3][3],

		m_Data[1][0] * rhs.m_Data[0][0] + m_Data[1][1] * rhs.m_Data[1][0] + m_Data[1][2] * rhs.m_Data[2][0] + m_Data[1][3] * rhs.m_Data[3][0],
		m_Data[1][0] * rhs.m_Data[0][1] + m_Data[1][1] * rhs.m_Data[1][1] + m_Data[1][2] * rhs.m_Data[2][1] + m_Data[1][3] * rhs.m_Data[3][1],
		m_Data[1][0] * rhs.m_Data[0][2] + m_Data[1][1] * rhs.m_Data[1][2] + m_Data[1][2] * rhs.m_Data[2][2] + m_Data[1][3] * rhs.m_Data[3][2],
		m_Data[1][0] * rhs.m_Data[0][3] + m_Data[1][1] * rhs.m_Data[1][3] + m_Data[1][2] * rhs.m_Data[2][3] + m_Data[1][3] * rhs.m_Data[3][3],

		m_Data[2][0] * rhs.m_Data[0][0] + m_Data[2][1] * rhs.m_Data[1][0] + m_Data[2][2] * rhs.m_Data[2][0] + m_Data[2][3] * rhs.m_Data[3][0],
		m_Data[2][0] * rhs.m_Data[0][1] + m_Data[2][1] * rhs.m_Data[1][1] + m_Data[2][2] * rhs.m_Data[2][1] + m_Data[2][3] * rhs.m_Data[3][1],
		m_Data[2][0] * rhs.m_Data[0][2] + m_Data[2][1] * rhs.m_Data[1][2] + m_Data[2][2] * rhs.m_Data[2][2] + m_Data[2][3] * rhs.m_Data[3][2],
		m_Data[2][0] * rhs.m_Data[0][3] + m_Data[2][1] * rhs.m_Data[1][3] + m_Data[2][2] * rhs.m_Data[2][3] + m_Data[2][3] * rhs.m_Data[3][3],

		m_Data[3][0] * rhs.m_Data[0][0] + m_Data[3][1] * rhs.m_Data[1][0] + m_Data[3][2] * rhs.m_Data[2][0] + m_Data[3][3] * rhs.m_Data[3][0],
		m_Data[3][0] * rhs.m_Data[0][1] + m_Data[3][1] * rhs.m_Data[1][1] + m_Data[3][2] * rhs.m_Data[2][1] + m_Data[3][3] * rhs.m_Data[3][1],
		m_Data[3][0] * rhs.m_Data[0][2] + m_Data[3][1] * rhs.m_Data[1][2] + m_Data[3][2] * rhs.m_Data[2][2] + m_Data[3][3] * rhs.m_Data[3][2],
		m_Data[3][0] * rhs.m_Data[0][3] + m_Data[3][1] * rhs.m_Data[1][3] + m_Data[3][2] * rhs.m_Data[2][3] + m_Data[3][3] * rhs.m_Data[3][3] };
}

// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
inline void Matrix::Inverse()
{
	float inv[4][4];
	inv[0][0] = 
		m_Data[1][1] * m_Data[2][2] * m_Data[3][3] -
		m_Data[1][1] * m_Data[2][3] * m_Data[3][2] -
		m_Data[2][1] * m_Data[1][2] * m_Data[3][3] +
		m_Data[2][1] * m_Data[1][3] * m_Data[3][2] +
		m_Data[3][1] * m_Data[1][2] * m_Data[2][3] -
		m_Data[3][1] * m_Data[1][3] * m_Data[2][2];

	inv[1][0] = 
		-m_Data[1][0] * m_Data[2][2] * m_Data[3][3] +
		m_Data[1][0] * m_Data[2][3] * m_Data[3][2] +
		m_Data[2][0] * m_Data[1][2] * m_Data[3][3] -
		m_Data[2][0] * m_Data[1][3] * m_Data[3][2] -
		m_Data[3][0] * m_Data[1][2] * m_Data[2][3] +
		m_Data[3][0] * m_Data[1][3] * m_Data[2][2];

	inv[2][0] = 
		m_Data[1][0] * m_Data[2][1] * m_Data[3][3] -
		m_Data[1][0] * m_Data[2][3] * m_Data[3][1] -
		m_Data[2][0] * m_Data[1][1] * m_Data[3][3] +
		m_Data[2][0] * m_Data[1][3] * m_Data[3][1] +
		m_Data[3][0] * m_Data[1][1] * m_Data[2][3] -
		m_Data[3][0] * m_Data[1][3] * m_Data[2][1];

	inv[3][0] = 
		-m_Data[1][0] * m_Data[2][1] * m_Data[3][2] +
		m_Data[1][0] * m_Data[2][2] * m_Data[3][1] +
		m_Data[2][0] * m_Data[1][1] * m_Data[3][2] -
		m_Data[2][0] * m_Data[1][2] * m_Data[3][1] -
		m_Data[3][0] * m_Data[1][1] * m_Data[2][2] +
		m_Data[3][0] * m_Data[1][2] * m_Data[2][1];

	inv[0][1] = 
		-m_Data[0][1] * m_Data[2][2] * m_Data[3][3] +
		m_Data[0][1] * m_Data[2][3] * m_Data[3][2] +
		m_Data[2][1] * m_Data[0][2] * m_Data[3][3] -
		m_Data[2][1] * m_Data[0][3] * m_Data[3][2] -
		m_Data[3][1] * m_Data[0][2] * m_Data[2][3] +
		m_Data[3][1] * m_Data[0][3] * m_Data[2][2];

	inv[1][1] = 
		m_Data[0][0] * m_Data[2][2] * m_Data[3][3] -
		m_Data[0][0] * m_Data[2][3] * m_Data[3][2] -
		m_Data[2][0] * m_Data[0][2] * m_Data[3][3] +
		m_Data[2][0] * m_Data[0][3] * m_Data[3][2] +
		m_Data[3][0] * m_Data[0][2] * m_Data[2][3] -
		m_Data[3][0] * m_Data[0][3] * m_Data[2][2];

	inv[2][1] = 
		-m_Data[0][0] * m_Data[2][1] * m_Data[3][3] +
		m_Data[0][0] * m_Data[2][3] * m_Data[3][1] +
		m_Data[2][0] * m_Data[0][1] * m_Data[3][3] -
		m_Data[2][0] * m_Data[0][3] * m_Data[3][1] -
		m_Data[3][0] * m_Data[0][1] * m_Data[2][3] +
		m_Data[3][0] * m_Data[0][3] * m_Data[2][1];

	inv[3][1] = 
		m_Data[0][0] * m_Data[2][1] * m_Data[3][2] -
		m_Data[0][0] * m_Data[2][2] * m_Data[3][1] -
		m_Data[2][0] * m_Data[0][1] * m_Data[3][2] +
		m_Data[2][0] * m_Data[0][2] * m_Data[3][1] +
		m_Data[3][0] * m_Data[0][1] * m_Data[2][2] -
		m_Data[3][0] * m_Data[0][2] * m_Data[2][1];

	inv[0][2] = 
		m_Data[0][1] * m_Data[1][2] * m_Data[3][3] -
		m_Data[0][1] * m_Data[1][3] * m_Data[3][2] -
		m_Data[1][1] * m_Data[0][2] * m_Data[3][3] +
		m_Data[1][1] * m_Data[0][3] * m_Data[3][2] +
		m_Data[3][1] * m_Data[0][2] * m_Data[1][3] -
		m_Data[3][1] * m_Data[0][3] * m_Data[1][2];

	inv[1][2] = 
		-m_Data[0][0] * m_Data[1][2] * m_Data[3][3] +
		m_Data[0][0] * m_Data[1][3] * m_Data[3][2] +
		m_Data[1][0] * m_Data[0][2] * m_Data[3][3] -
		m_Data[1][0] * m_Data[0][3] * m_Data[3][2] -
		m_Data[3][0] * m_Data[0][2] * m_Data[1][3] +
		m_Data[3][0] * m_Data[0][3] * m_Data[1][2];

	inv[2][2] = 
		m_Data[0][0] * m_Data[1][1] * m_Data[3][3] -
		m_Data[0][0] * m_Data[1][3] * m_Data[3][1] -
		m_Data[1][0] * m_Data[0][1] * m_Data[3][3] +
		m_Data[1][0] * m_Data[0][3] * m_Data[3][1] +
		m_Data[3][0] * m_Data[0][1] * m_Data[1][3] -
		m_Data[3][0] * m_Data[0][3] * m_Data[1][1];

	inv[3][2] = 
		-m_Data[0][0] * m_Data[1][1] * m_Data[3][2] +
		m_Data[0][0] * m_Data[1][2] * m_Data[3][1] +
		m_Data[1][0] * m_Data[0][1] * m_Data[3][2] -
		m_Data[1][0] * m_Data[0][2] * m_Data[3][1] -
		m_Data[3][0] * m_Data[0][1] * m_Data[1][2] +
		m_Data[3][0] * m_Data[0][2] * m_Data[1][1];

	inv[0][3] = 
		-m_Data[0][1] * m_Data[1][2] * m_Data[2][3] +
		m_Data[0][1] * m_Data[1][3] * m_Data[2][2] +
		m_Data[1][1] * m_Data[0][2] * m_Data[2][3] -
		m_Data[1][1] * m_Data[0][3] * m_Data[2][2] -
		m_Data[2][1] * m_Data[0][2] * m_Data[1][3] +
		m_Data[2][1] * m_Data[0][3] * m_Data[1][2];

	inv[1][3] = 
		m_Data[0][0] * m_Data[1][2] * m_Data[2][3] -
		m_Data[0][0] * m_Data[1][3] * m_Data[2][2] -
		m_Data[1][0] * m_Data[0][2] * m_Data[2][3] +
		m_Data[1][0] * m_Data[0][3] * m_Data[2][2] +
		m_Data[2][0] * m_Data[0][2] * m_Data[1][3] -
		m_Data[2][0] * m_Data[0][3] * m_Data[1][2];

	inv[2][3] = 
		-m_Data[0][0] * m_Data[1][1] * m_Data[2][3] +
		m_Data[0][0] * m_Data[1][3] * m_Data[2][1] +
		m_Data[1][0] * m_Data[0][1] * m_Data[2][3] -
		m_Data[1][0] * m_Data[0][3] * m_Data[2][1] -
		m_Data[2][0] * m_Data[0][1] * m_Data[1][3] +
		m_Data[2][0] * m_Data[0][3] * m_Data[1][1];

	inv[3][3] = 
		m_Data[0][0] * m_Data[1][1] * m_Data[2][2] -
		m_Data[0][0] * m_Data[1][2] * m_Data[2][1] -
		m_Data[1][0] * m_Data[0][1] * m_Data[2][2] +
		m_Data[1][0] * m_Data[0][2] * m_Data[2][1] +
		m_Data[2][0] * m_Data[0][1] * m_Data[1][2] -
		m_Data[2][0] * m_Data[0][2] * m_Data[1][1];

	const float det = 1.f / (
		m_Data[0][0] * inv[0][0] + 
		m_Data[0][1] * inv[1][0] + 
		m_Data[0][2] * inv[2][0] + 
		m_Data[0][3] * inv[3][0] );

	for (int8_t i = 0; i < 4; ++i)
		for (int8_t j = 0; j < 4; ++j)
			inv[i][j] *= det;

	std::memcpy(&m_Data, &inv, sizeof(float) * 16);
}

inline void Matrix::Transpose()
{
	std::swap(m_Data[0][1], m_Data[1][0]);
	std::swap(m_Data[0][2], m_Data[2][0]);
	std::swap(m_Data[0][3], m_Data[3][0]);

	std::swap(m_Data[1][2], m_Data[2][1]);
	std::swap(m_Data[1][3], m_Data[3][1]);

	std::swap(m_Data[2][3], m_Data[3][2]);
}

inline Matrix Matrix::Inversed() const
{
	Matrix value(*this);
	value.Inverse();
	return value;
}

inline Matrix Matrix::Transposed() const
{
	Matrix value(*this);
	value.Transpose();
	return value;
}

inline void Matrix::SetTranslate(const vec3& value)
{
	m_Data[3][0] = value.x; 
	m_Data[3][1] = value.y; 
	m_Data[3][2] = value.z; 
}

inline Matrix Matrix::CreatePerspective(float aspectRatio, float nearZ, float farZ)
{
	Matrix perspective = Matrix::Identity;
	constexpr float fovy = Math::ToRadians(50.f);
	const float tanHalfFovy = tanf(fovy / 2.f);
	perspective[0][0] = 1.f / (aspectRatio * tanHalfFovy);
	perspective[1][1] = 1.f / (tanHalfFovy);
	perspective[2][2] = farZ / (farZ - nearZ);
	perspective[2][3] = 1.f;
	perspective[3][2] = -(farZ * nearZ) / (farZ - nearZ);
	
	return perspective;
}

inline void Matrix::SetRotate(const Quaternion& value)
{
	const float s0 = vec3(m_Data[0][0], m_Data[0][1], m_Data[0][2]).Length();
	const float s1 = vec3(m_Data[1][0], m_Data[1][1], m_Data[1][2]).Length();
	const float s2 = vec3(m_Data[2][0], m_Data[2][1], m_Data[2][2]).Length();

	m_Data[0][0] = (1.f - (2.f * value.y * value.y) - (2.f * value.z * value.z)) * s0;
	m_Data[0][1] = ((2.f * value.x * value.y) + (2.f * value.z * value.w)) * s0;
	m_Data[0][2] = ((2.f * value.x * value.z) - (2.f * value.y * value.w)) * s0;

	m_Data[1][0] = ((2.f * value.x * value.y) - (2.f * value.z * value.w)) * s1;
	m_Data[1][1] = (1.f - (2.f * value.x * value.x) - (2.f * value.z * value.z)) * s1;
	m_Data[1][2] = ((2.f * value.y * value.z) + (2.f * value.x * value.w)) * s1;

	m_Data[2][0] = ((2.f * value.x * value.z) + (2.f * value.y * value.w)) * s2;
	m_Data[2][1] = ((2.f * value.y * value.z) - (2.f * value.x * value.w)) * s2;
	m_Data[2][2] = (1.f - (2.f * value.x * value.x) - (2.f * value.y * value.y)) * s2;
}

inline void Matrix::SetScale(const vec3& value)
{
	const vec3 r0 = vec3(m_Data[0][0], m_Data[0][1], m_Data[0][2]).GetNormalized();
	const vec3 r1 = vec3(m_Data[1][0], m_Data[1][1], m_Data[1][2]).GetNormalized();
	const vec3 r2 = vec3(m_Data[2][0], m_Data[2][1], m_Data[2][2]).GetNormalized();
	m_Data[0][0] = r0.x * value.x;
	m_Data[1][1] = r1.y * value.y;
	m_Data[2][2] = r2.z * value.z;
}

inline void Matrix::SetScale(const float value)
{
	const vec3 r0 = vec3(m_Data[0][0], m_Data[0][1], m_Data[0][2]).GetNormalized();
	const vec3 r1 = vec3(m_Data[1][0], m_Data[1][1], m_Data[1][2]).GetNormalized();
	const vec3 r2 = vec3(m_Data[2][0], m_Data[2][1], m_Data[2][2]).GetNormalized();
	m_Data[0][0] = r0.x * value;
	m_Data[1][1] = r1.y * value;
	m_Data[2][2] = r2.z * value;
}

inline Matrix Matrix::FromTranslate(const vec3& value)
{
	return Matrix{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		value.x, value.y, value.z, 1.f };
}

inline Matrix Matrix::FromRotate(const Quaternion& value)
{
	return Matrix{
		1.f - (2.f * value.y * value.y) - (2.f * value.z * value.z),
		(2.f * value.x * value.y) + (2.f * value.z * value.w),
		(2.f * value.x * value.z) - (2.f * value.y * value.w),
		0.f,

		(2.f * value.x * value.y) - (2.f * value.z * value.w),
		1.f - (2.f * value.x * value.x) - (2.f * value.z * value.z),
		(2.f * value.y * value.z) + (2.f * value.x * value.w),
		0.f,

		(2.f * value.x * value.z) + (2.f * value.y * value.w),
		(2.f * value.y * value.z) - (2.f * value.x * value.w),
		1.f - (2.f * value.x * value.x) - (2.f * value.y * value.y),
		0.f,

		0.f,
		0.f,
		0.f,
		1.f };
}

inline Matrix Matrix::FromScale(const vec3& value)
{
	return Matrix{
		value.x, 0.f, 0.f, 0.f,
		0.f, value.y, 0.f, 0.f,
		0.f, 0.f, value.z, 0.f,
		0.f, 0.f, 0.f, 1.f };
}

inline Matrix Matrix::FromScale(const float value)
{
	return Matrix{
		value, 0.f, 0.f, 0.f,
		0.f, value, 0.f, 0.f,
		0.f, 0.f, value, 0.f,
		0.f, 0.f, 0.f, 1.f };
}

inline vec2 operator*(const vec2& lhs, const Matrix& rhs)
{
	return vec2(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[3][0],
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[3][1]);
}

inline vec3 operator*(const vec3& lhs, const Matrix& rhs)
{
	return vec3(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[2][0] * lhs.z + rhs.m_Data[3][0],
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[2][1] * lhs.z + rhs.m_Data[3][1],
		rhs.m_Data[0][2] * lhs.x + rhs.m_Data[1][2] * lhs.y + rhs.m_Data[2][2] * lhs.z + rhs.m_Data[3][2]);
}

inline vec4 operator*(const vec4& lhs, const Matrix& rhs)
{
	return vec4(
		rhs.m_Data[0][0] * lhs.x + rhs.m_Data[1][0] * lhs.y + rhs.m_Data[2][0] * lhs.z + rhs.m_Data[3][0] * lhs.w,
		rhs.m_Data[0][1] * lhs.x + rhs.m_Data[1][1] * lhs.y + rhs.m_Data[2][1] * lhs.z + rhs.m_Data[3][1] * lhs.w,
		rhs.m_Data[0][2] * lhs.x + rhs.m_Data[1][2] * lhs.y + rhs.m_Data[2][2] * lhs.z + rhs.m_Data[3][2] * lhs.w,
		rhs.m_Data[0][3] * lhs.x + rhs.m_Data[1][3] * lhs.y + rhs.m_Data[2][3] * lhs.z + rhs.m_Data[3][3] * lhs.w);
}

inline bool IsNearly(const Matrix& a, const Matrix& b, const float epsilon /*= 0.001f*/)
{
	return abs(a.m_Data[0][0] - b.m_Data[0][0]) < epsilon
		&& abs(a.m_Data[0][1] - b.m_Data[0][1]) < epsilon
		&& abs(a.m_Data[0][2] - b.m_Data[0][2]) < epsilon
		&& abs(a.m_Data[0][3] - b.m_Data[0][3]) < epsilon

		&& abs(a.m_Data[1][0] - b.m_Data[1][0]) < epsilon
		&& abs(a.m_Data[1][1] - b.m_Data[1][1]) < epsilon
		&& abs(a.m_Data[1][2] - b.m_Data[1][2]) < epsilon
		&& abs(a.m_Data[1][3] - b.m_Data[1][3]) < epsilon

		&& abs(a.m_Data[2][0] - b.m_Data[2][0]) < epsilon
		&& abs(a.m_Data[2][1] - b.m_Data[2][1]) < epsilon
		&& abs(a.m_Data[2][2] - b.m_Data[2][2]) < epsilon
		&& abs(a.m_Data[2][3] - b.m_Data[2][3]) < epsilon

		&& abs(a.m_Data[3][0] - b.m_Data[3][0]) < epsilon
		&& abs(a.m_Data[3][1] - b.m_Data[3][1]) < epsilon
		&& abs(a.m_Data[3][2] - b.m_Data[3][2]) < epsilon
		&& abs(a.m_Data[3][3] - b.m_Data[3][3]) < epsilon;
}