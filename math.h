#pragma once
#include "main.h"

constexpr auto PI = (3.14159265359);

inline float Dot(XMFLOAT3 vec1, XMFLOAT3 vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline float LengthSquared(const XMFLOAT3 vec)
{
	return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
}
inline float Length(const XMFLOAT3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}
inline XMFLOAT3 Cross(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	return XMFLOAT3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

inline XMFLOAT3 Normalize(XMFLOAT3 vec)
{
	float len = Length(vec);

	if (len != 0.0f)
	{
		return XMFLOAT3(vec.x / len, vec.y / len, vec.z / len);
	}
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

inline float Lerp(float vector, float target, float speed)
{
	return vector + (target - vector) * speed;
}

inline XMFLOAT3 Sub(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return{ vec1.x - vec2.x,vec1.y - vec2.y,vec1.z - vec2.z };
}
inline XMFLOAT3 Add(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return{ vec1.x + vec2.x,vec1.y + vec2.y,vec1.z + vec2.z };
}
inline XMFLOAT3 Mul(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return{ vec1.x * vec2.x,vec1.y * vec2.y,vec1.z * vec2.z };
}

inline XMFLOAT3 MulFloat(XMFLOAT3 vec, float f)
{
	return { vec.x * f,vec.y * f,vec.z * f };
}


