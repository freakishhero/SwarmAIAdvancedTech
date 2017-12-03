#pragma once
#include <cmath>
#include <DirectXMath.h>

struct Vector3
{
	float x, y, z;

	inline Vector3(void) {}
	inline Vector3(const float _x, const float _y, const float _z)
	{
		x = _x; y = _y; z = _z;
	}

	inline Vector3 operator + (const Vector3& _vector3) const
	{
		return Vector3(x + _vector3.x, y + _vector3.y, z + _vector3.z);
	}

	inline Vector3 operator += (const Vector3& _vector3) const
	{
		return Vector3(x + _vector3.x, y + _vector3.y, z + _vector3.z);
	}

	inline Vector3 operator - (const Vector3& _vector3) const
	{
		return Vector3(x - _vector3.x, y - _vector3.y, z - _vector3.z);
	}


	inline Vector3 operator + (const float _vector3) const
	{
		return Vector3(x + _vector3, y + _vector3, z + _vector3);
	}

	inline Vector3 operator * (const Vector3& _vector3) const
	{
		return Vector3(x * _vector3.x, y * _vector3.y, z * _vector3.z);
	}

	inline Vector3 operator * (const float _multiplier) const
	{
		return Vector3(x * _multiplier, y * _multiplier, z * _multiplier);
	}


	inline float Dot(const Vector3& _vector3) const
	{
		return _vector3.x*x + _vector3.y*y + _vector3.z*z;
	}

	inline float Magnitude(const Vector3& _vector3)
	{
		return sqrt((_vector3.x * _vector3.x) + (_vector3.y * _vector3.y) + (_vector3.z * _vector3.z));
	}

	inline float Distance(const Vector3& vector1, const Vector3& vector2)
	{
		float _z_difference = vector1.z - vector2.z;
		float _y_difference = vector1.y - vector2.y;
		float _x_difference = vector1.x - vector2.x;
		return sqrt((_z_difference * _z_difference) + (_y_difference * _y_difference) + (_x_difference * _x_difference));
	}
};