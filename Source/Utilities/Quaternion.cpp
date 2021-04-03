#include "Quaternion.h"
#include <cmath>
Quaternion::Quaternion(const float x, const float y, const float z, const float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Quaternion::normalize()
{
	float m = std::sqrtf(w * w + x * x + y * y + z * z);
	x /= m;
	y /= m;
	z /= m;
	w /= m;
}

Matrix4f Quaternion::toRotationMatrix()
{
	Matrix4f m;
	const float xy = x * y;
	const float xz = x * z;
	const float xw = x * w;
	const float yz = y * z;
	const float yw = y * w;
	const float zw = z * w;
	const float xSquared = x * x;
	const float ySquared = y * y;
	const float zSquared = z * z;
	m.m00 = 1 - 2 * (ySquared + zSquared);
	m.m01 = 2 * (xy - zw);
	m.m02 = 2 * (xz + yw);
	m.m03 = 0;
	m.m10 = 2 * (xy + zw);
	m.m11 = 1 - 2 * (xSquared + zSquared);
	m.m12 = 2 * (yz - xw);
	m.m13 = 0;
	m.m20 = 2 * (xz - yw);
	m.m21 = 2 * (yz + xw);
	m.m22 = 1 - 2 * (xSquared + ySquared);
	m.m23 = 0;
	m.m30 = 0;
	m.m31 = 0;
	m.m32 = 0;
	m.m33 = 1;
	return m;
}

Quaternion Quaternion::fromMatrix(const Matrix4f& m)
{
	float w, x, y, z;
	float diagonal = m.m00 + m.m11 + m.m22;
	if (diagonal > 0)
	{
		float w4 = (float)(std::sqrtf(diagonal + 1.0f) * 2.0f);
		w = w4 / 4.0f;
		x = (m.m21 - m.m12) / w4;
		y = (m.m02 - m.m20) / w4;
		z = (m.m10 - m.m01) / w4;
	}
	else if ((m.m00 > m.m11) && (m.m00 > m.m22))
	{
		float x4 = (float)(std::sqrtf(1.0f + m.m00 - m.m11 - m.m22) * 2.0f);
		w = (m.m21 - m.m12) / x4;
		x = x4 / 4.0f;
		y = (m.m01 + m.m10) / x4;
		z = (m.m02 + m.m20) / x4;
	}
	else if (m.m11 > m.m22)
	{
		float y4 = (float)(std::sqrtf(1.0f + m.m11 - m.m00 - m.m22) * 2.0f);
		w = (m.m02 - m.m20) / y4;
		x = (m.m01 + m.m10) / y4;
		y = y4 / 4.0f;
		z = (m.m12 + m.m21) / y4;
	}
	else {
		float z4 = (float)(std::sqrtf(1.0f + m.m22 - m.m00 - m.m11) * 2.0f);
		w = (m.m10 - m.m01) / z4;
		x = (m.m02 + m.m20) / z4;
		y = (m.m12 + m.m21) / z4;
		z = z4 / 4.0f;
	}
	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::interpolate(const Quaternion& a, const Quaternion& b, const float blend)
{
	Quaternion result = Quaternion(0, 0, 0, 1);
	float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	float blendI = 1.0f - blend;
	if (dot < 0)
	{
		result.w = blendI * a.w + blend * -b.w;
		result.x = blendI * a.x + blend * -b.x;
		result.y = blendI * a.y + blend * -b.y;
		result.z = blendI * a.z + blend * -b.z;
	}
	else
	{
		result.w = blendI * a.w + blend * b.w;
		result.x = blendI * a.x + blend * b.x;
		result.y = blendI * a.y + blend * b.y;
		result.z = blendI * a.z + blend * b.z;
	}
	result.normalize();
	return result;
}
