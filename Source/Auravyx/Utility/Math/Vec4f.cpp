#include "MSVC/pch.h"
#include "Auravyx/Utility/Math/Vec4f.h"

Vec4f::Vec4f(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4f::Vec4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Vec4f::~Vec4f()
{
}

float Vec4f::getX()
{
	return x;
}

float Vec4f::getY()
{
	return y;
}

float Vec4f::getZ()
{
	return z;
}

void Vec4f::add(float xN, float yN, float zN)
{
	x += xN;
	y += yN;
	z += zN;
}

void Vec4f::subtract(float x, float y, float z)
{
	this->x -= x;
	this->y -= y;
	this->z -= z;
}

void Vec4f::normalize()
{
	//Vec4f n = M::normalize(Vec4f(x, y, z));
	//x = n.x;
	//y = n.y;
	//z = n.z;
}

void Vec4f::add(Vec4f v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

Vec4f Vec4f::rgbaColour(float r, float g, float b, float a)
{
	return Vec4f(r / (float) 255, g / (float) 255, b / (float) 255, a / (float) 255);
}
Vec4f& Vec4f::operator+=(const Vec4f b)
{
	this->x += b.x;
	this->y += b.y;
	this->z += b.z;
	this->w += b.w;
	return *this;
}
Vec4f& Vec4f::operator-=(const Vec4f b)
{
	this->x -= b.x;
	this->y -= b.y;
	this->z -= b.z;
	this->w -= b.w;
	return *this;
}
Vec4f& Vec4f::operator*=(const Vec4f b)
{
	this->x *= b.x;
	this->y *= b.y;
	this->z *= b.z;
	this->w *= b.w;
	return *this;
}
Vec4f& Vec4f::operator/=(const Vec4f b)
{
	this->x /= b.x;
	this->y /= b.y;
	this->z /= b.z;
	this->w /= b.w;
	return *this;
}

Vec4f& Vec4f::operator+=(const float b)
{
	this->x += b;
	this->y += b;
	this->z += b;
	this->w += b;
	return *this;
}
Vec4f& Vec4f::operator-=(const float b)
{
	this->x -= b;
	this->y -= b;
	this->z -= b;
	this->w -= b;
	return *this;
}
Vec4f& Vec4f::operator*=(const float b)
{
	this->x *= b;
	this->y *= b;
	this->z *= b;
	this->w *= b;
	return *this;
}
Vec4f& Vec4f::operator/=(const float b)
{
	this->x /= b;
	this->y /= b;
	this->z /= b;
	this->w /= b;
	return *this;
}

Vec4f operator+(const Vec4f a, const Vec4f b)
{
	return Vec4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vec4f operator-(const Vec4f a, const Vec4f b)
{
	return Vec4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Vec4f operator*(const Vec4f a, const Vec4f b)
{
	return Vec4f(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

Vec4f operator/(const Vec4f a, const Vec4f b)
{
	return Vec4f(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

Vec4f operator+(const Vec4f a, const float b)
{
	return Vec4f(a.x + b, a.y + b, a.z + b, a.w + b);
}

Vec4f operator-(const Vec4f a, const float b)
{
	return Vec4f(a.x - b, a.y - b, a.z - b, a.w - b);
}

Vec4f operator*(const Vec4f a, const float b)
{
	return Vec4f(a.x * b, a.y * b, a.z * b, a.w * b);
}

Vec4f operator/(const Vec4f a, const float b)
{
	return Vec4f(a.x / b, a.y / b, a.z / b, a.w * b);
}