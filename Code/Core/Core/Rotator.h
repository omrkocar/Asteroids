#pragma once

class vec3;

class Rotator
{
public:
	constexpr Rotator() noexcept : pitch(), yaw(), roll() {}
	constexpr Rotator(const float value) noexcept : pitch(value), yaw(value), roll(value) {}
	constexpr Rotator(const float pitch, const float yaw, const float roll) noexcept : pitch(pitch), yaw(yaw), roll(roll) {}
	constexpr Rotator(const vec3& value) noexcept;

	bool operator==(const Rotator& rhs) const noexcept { return (pitch == rhs.pitch) && (yaw == rhs.yaw) && (roll == rhs.roll); }
	bool operator!=(const Rotator& rhs) const noexcept { return (pitch != rhs.pitch) || (yaw != rhs.yaw) || (roll != rhs.roll); }

	Rotator operator+(const Rotator& rhs) const noexcept { return Rotator(pitch + rhs.pitch, yaw + rhs.yaw, roll + rhs.roll); }
	Rotator operator-(const Rotator& rhs) const noexcept { return Rotator(pitch - rhs.pitch, yaw - rhs.yaw, roll - rhs.roll); }

	Rotator& operator+=(const Rotator& rhs) noexcept { pitch += rhs.pitch; yaw += rhs.yaw; roll += rhs.roll; return *this; }
	Rotator& operator-=(const Rotator& rhs) noexcept { pitch -= rhs.pitch; yaw -= rhs.yaw; roll -= rhs.roll; return *this; }

	Rotator operator*(const float rhs) const noexcept { return Rotator(pitch * rhs, yaw * rhs, roll * rhs); }
	Rotator operator/(const float rhs) const noexcept { return Rotator(pitch / rhs, yaw / rhs, roll / rhs); }

	Rotator& operator*=(const float rhs) noexcept { pitch *= rhs; yaw *= rhs; roll *= rhs; return *this; }
	Rotator& operator/=(const float rhs) noexcept { pitch /= rhs; yaw /= rhs; roll /= rhs; return *this; }

	Rotator operator-() const noexcept { return Rotator(-pitch, -yaw, -roll); }

	static const Rotator Zero;

public:
	float pitch, yaw, roll;
};